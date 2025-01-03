$repoDir = (Get-Item -Path $PSScriptRoot).Parent.FullName
$binDir = "$repoDir\bin"
$srcDir = "$repoDir\src"
$docsDir = "$repoDir\ci\docs"
$latexDir = "$docsDir\latex"
$xmlDir = "$docsDir\xml"
$htmlDir = "$docsDir\html"

$dirsToCreate = @($binDir, $docsDir, $xmlDir, $htmlDir, $latexDir)
foreach ($dir in $dirsToCreate) {
    if (-not (Test-Path $dir)) {
        Write-Host "`n--------------------------------------------------`n"
        Write-Host "Creating directory: $dir"
        Write-Host "`n--------------------------------------------------`n"
        New-Item -ItemType Directory -Path $dir | Out-Null
    }
}

$doxygenUrl = "https://github.com/doxygen/doxygen/releases/download/Release_1_12_0/doxygen-1.12.0.windows.x64.bin.zip"
$doxygenZipFile = "$binDir\doxygen.zip"
$doxygenDir = "$binDir\doxy"
$doxygenExeFile = "$doxygenDir\doxygen.exe"

if (-not (Test-Path $doxygenExeFile)) {
    Write-Host "`n--------------------------------------------------`n"
    Write-Host "Doxygen not found, downloading..."
    Write-Host "`n--------------------------------------------------`n"
    Invoke-WebRequest -Uri $doxygenUrl -OutFile $doxygenZipFile
    Write-Host "`n--------------------------------------------------`n"
    Write-Host "Downloaded Doxygen to $doxygenZipFile"
    Write-Host "`n--------------------------------------------------`n"
    Expand-Archive -Path $doxygenZipFile -DestinationPath $doxygenDir -Force
    Write-Host "`n--------------------------------------------------`n"
    Write-Host "Expanded Doxygen to $doxygenDir"
    Write-Host "`n--------------------------------------------------`n"
} else {
    Write-Host "`n--------------------------------------------------`n"
    Write-Host "Doxygen already installed at $doxygenDir"
    Write-Host "`n--------------------------------------------------`n"
}

$env:Path += ";$doxygenDir"
$doxygenConfigFile = "$docsDir\Doxyfile"
if (-not (Test-Path $doxygenConfigFile)) {
    & $doxygenExeFile -g $doxygenConfigFile
}

$projectName = "csad2425ki405KononOV13"

(Get-Content $doxygenConfigFile) -replace 'PROJECT_NAME\s*=\s*\".*\"', "PROJECT_NAME = `"$projectName`"" | Set-Content $doxygenConfigFile
(Get-Content $doxygenConfigFile) -replace 'INPUT\s*=\s*.*', "INPUT = $srcDir" | Set-Content $doxygenConfigFile
(Get-Content $doxygenConfigFile) -replace 'RECURSIVE\s*=\s*.*', "RECURSIVE = YES" | Set-Content $doxygenConfigFile
(Get-Content $doxygenConfigFile) -replace 'EXCLUDE_PATTERNS\s*=\s*.*', "EXCLUDE_PATTERNS = *AssemblyAttributes.cpp" | Set-Content $doxygenConfigFile
(Get-Content $doxygenConfigFile) -replace 'GENERATE_XML\s*=\s*NO', "GENERATE_XML = YES" | Set-Content $doxygenConfigFile
(Get-Content $doxygenConfigFile) -replace 'XML_OUTPUT\s*=\s*.*', "XML_OUTPUT = $xmlDir" | Set-Content $doxygenConfigFile
(Get-Content $doxygenConfigFile) -replace 'GENERATE_HTML\s*=\s*NO', "GENERATE_HTML = YES" | Set-Content $doxygenConfigFile
(Get-Content $doxygenConfigFile) -replace 'HTML_OUTPUT\s*=\s*.*', "HTML_OUTPUT = $htmlDir" | Set-Content $doxygenConfigFile
(Get-Content $doxygenConfigFile) -replace 'LATEX_OUTPUT\s*=\s*.*', "LATEX_OUTPUT = $latexDir" | Set-Content $doxygenConfigFile
(Get-Content $doxygenConfigFile) -replace 'EXTRACT_ALL\s*=\s*NO', "EXTRACT_ALL = YES" | Set-Content $doxygenConfigFile
(Get-Content $doxygenConfigFile) -replace 'EXTRACT_PRIVATE\s*=\s*NO', "EXTRACT_PRIVATE = YES" | Set-Content $doxygenConfigFile
(Get-Content $doxygenConfigFile) -replace 'EXTRACT_STATIC\s*=\s*NO', "EXTRACT_STATIC = YES" | Set-Content $doxygenConfigFile
(Get-Content $doxygenConfigFile) -replace "FILE_PATTERNS\s*=\s*.*", "FILE_PATTERNS = *.ino\" | Set-Content $doxygenConfigFile
(Get-Content $doxygenConfigFile) -replace 'EXTENSION_MAPPING\s*=\s*.*', 'EXTENSION_MAPPING = ino=C++' | Set-Content $doxygenConfigFile
(Get-Content $doxygenConfigFile) -replace 'EXCLUDE\s*=\s*.*', "EXCLUDE = tinyxml2.h tinyxml2.cpp" | Set-Content $doxygenConfigFile
(Get-Content $doxygenConfigFile) -replace 'EXCLUDE_PATTERNS\s*=\s*.*', "EXCLUDE_PATTERNS = *tinyxml2.h *tinyxml2.cpp" | Set-Content $doxygenConfigFile


& $doxygenExeFile $doxygenConfigFile
Write-Host "`n--------------------------------------------------`n"
Write-Host "Documentation generated in $docsDir"
Write-Host "`n`n`n--------------------------------------------------`n"