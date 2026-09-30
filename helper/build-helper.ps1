$ErrorActionPreference="Stop"
$root=Split-Path -Parent $MyInvocation.MyCommand.Path
$venv=Join-Path $root ".venv"
if(!(Test-Path $venv)){python -m venv $venv}
$py=Join-Path $venv "Scripts/python.exe"
& $py -m pip install --upgrade pip
& $py -m pip install -r "$root/requirements.txt" -r "$root/requirements-dev.txt"
$env:PYTHONPATH=Join-Path $root "src"
& $py -m pytest "$root/../tests/python"
& $py -m PyInstaller --noconfirm --clean "$root/Jinja2Tools.Helper.spec" --distpath "$root/dist" --workpath "$root/build"
if(!(Test-Path "$root/dist/Jinja2Tools.Helper.exe")){throw "Helper executable was not created."}
