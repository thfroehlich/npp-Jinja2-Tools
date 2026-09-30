param([Parameter(Mandatory=$true)][string]$NotepadPlusPlusPath)
$ErrorActionPreference="Stop";$src=Join-Path $PSScriptRoot "../dist/Jinja2Tools";$dst=Join-Path $NotepadPlusPlusPath "plugins/Jinja2Tools";New-Item -ItemType Directory -Force $dst|Out-Null;Copy-Item "$src/*" $dst -Force
