param (
    [string]$port
)

$repoDir = (Get-Item -Path $PSScriptRoot).Parent.FullName
$binDir = "$repoDir\bin"
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

if ($port) {
    Write-Host "`n--------------------------------------------------`n"
    Write-Host "Uploading HEX file to Arduino on port $port..."
    Write-Host "`n--------------------------------------------------`n"
    Write-Host "`n--------------------------------------------------`n"
    & $arduinoCliPath upload -p $port --fqbn arduino:avr:uno -i $outputHexPath
    Write-Host "`n--------------------------------------------------`n"
}
else {
    Write-Host "`n--------------------------------------------------`n"
    Write-Host "No COM port provided. Skipping upload and hardware tests."
    Write-Host "`n--------------------------------------------------`n"
}
Write-Host "`n`n`n--------------------------------------------------`n"