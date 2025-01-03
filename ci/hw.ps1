



param (
    [string]$port,
    [string]$speed
)

function Find-MSBuild {
    $possiblePaths = @(
        "C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe",
        "C:\Program Files (x86)\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe",
        "C:\Program Files\Microsoft Visual Studio\2022\Professional\MSBuild\Current\Bin\MSBuild.exe",
        "C:\Program Files (x86)\Microsoft Visual Studio\2022\Professional\MSBuild\Current\Bin\MSBuild.exe",
        "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\MSBuild\Current\Bin\MSBuild.exe",
        "C:\Program Files (x86)\Microsoft Visual Studio\2022\Enterprise\MSBuild\Current\Bin\MSBuild.exe"
    )

    foreach ($path in $possiblePaths) {
        if (Test-Path $path) {
            return $path
        }
    }

    Write-Error "MSBuild.exe not found. Please ensure it is installed and the path is correct."
    exit 1
}
$repoDir = (Get-Item -Path $PSScriptRoot).Parent.FullName
$binDir = "$repoDir\bin"
$outputProjectTestServerDir = "$binDir\test_server"
$projectTestHWPath = "$repoDir\tests\test_server\test_server.sln"
$testResultsDir = "$repoDir\ci\test_result"
$testResultPath = "../../test_result/TestResult.xml"
$resultTestServerDir = "$repoDir\ci\test_result\server"
$hexOutputDir = "$repoDir\ci\build\server"
$arduinoCliPath = "$binDir\arduino-cli.exe"
$sketchPath = "$repoDir\src\server\server.ino"
$outputHexPath = "$hexOutputDir\server-script-arduino-uno-r3.ino.hex"

if (-not (Test-Path $arduinoCliPath)) {
    Write-Host "`n--------------------------------------------------`n"
    Write-Host "Arduino CLI not found, proceeding with installation..."
    Write-Host "`n--------------------------------------------------`n"
    New-Item -ItemType Directory -Force -Path $binDir | Out-Null
    Invoke-WebRequest -Uri "https://downloads.arduino.cc/arduino-cli/arduino-cli_latest_Windows_64bit.zip" -OutFile "$binDir\arduino-cli.zip"
    Expand-Archive -Path "$binDir\arduino-cli.zip" -DestinationPath $binDir -Force
    Remove-Item "$binDir\arduino-cli.zip"
} else {
    Write-Host "`n--------------------------------------------------`n"
    Write-Host "Arduino CLI is already installed."
    Write-Host "`n--------------------------------------------------`n"
}

Write-Host "`n--------------------------------------------------`n"
Write-Host "Configuring Arduino CLI now..."
Write-Host "`n--------------------------------------------------`n"

& $arduinoCliPath config init
& $arduinoCliPath core update-index
& $arduinoCliPath core install arduino:avr

Write-Host "`n--------------------------------------------------`n"
Write-Host "Compiling Arduino sketch..."
Write-Host "`n--------------------------------------------------`n"

New-Item -ItemType Directory -Force -Path $hexOutputDir | Out-Null

& $arduinoCliPath compile --fqbn arduino:avr:uno --output-dir $hexOutputDir $sketchPath

Write-Host "`n--------------------------------------------------`n"
Write-Host "Saving compiled HEX file..."
Write-Host "`n--------------------------------------------------`n"
Copy-Item -Path "$hexOutputDir\server.ino.hex" -Destination $outputHexPath -Force

if ($port -and $speed) {
    Write-Host "`n--------------------------------------------------`n"
    Write-Host "Uploading HEX file to Arduino on port $port..."
    Write-Host "`n--------------------------------------------------`n"
    Write-Host "`n--------------------------------------------------`n"
    & $arduinoCliPath upload -p $port --fqbn arduino:avr:uno -i $outputHexPath
    Write-Host "`n--------------------------------------------------`n"

    $msbuildPath = Find-MSBuild
    Write-Host "`n--------------------------------------------------`n"
    Write-Host "Building the hardware testing project..."
    Write-Host "`n--------------------------------------------------`n"
    & $msbuildPath $projectTestHWPath "/p:OutDir=$outputProjectTestServerDir\"
    $exePath = "$outputProjectTestServerDir\test_server.exe"
    $arguments = @(
        $port,
        $speed,
        "--gtest_output=xml:$resultTestServerDir/TestResultServer.xml")
    Write-Host "`n--------------------------------------------------`n"
    Write-Host "Running hardware tests..."
    Write-Host "`n--------------------------------------------------`n"
    & $exePath $arguments
    $xmlPath = "$resultTestServerDir/TestResultServer.xml"
    $xsltPath = "$testResultsDir\TestResultsToHTML.xslt"
    $htmlOutputPath = "$resultTestServerDir/TestResult.html"
    Write-Host "`n--------------------------------------------------`n"
    Write-Host "Transforming test results to HTML format..."
    Write-Host "`n--------------------------------------------------`n"
    $xml = New-Object System.Xml.XmlDocument
    $xml.Load($xmlPath)
    $xslt = New-Object System.Xml.Xsl.XslCompiledTransform
    $xslt.Load($xsltPath)
    $xslt.Transform($xmlPath, $htmlOutputPath)
    Write-Host "`n--------------------------------------------------`n"
    Write-Host "Test results are available at: $htmlOutputPath"
    Write-Host "`n--------------------------------------------------`n"
}
else {
    Write-Host "`n--------------------------------------------------`n"
    Write-Host "No COM port provided. Skipping upload and hardware tests."
    Write-Host "`n--------------------------------------------------`n"
}
Write-Host "`n`n`n--------------------------------------------------`n"