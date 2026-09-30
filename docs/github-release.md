# Automated GitHub release build

The workflow `.github/workflows/release.yml` builds the helper and DLL, runs tests, creates a ZIP and checksum, and publishes a GitHub Release for semantic-version tags.

## Tag release

```powershell
git tag v0.2.0
git push origin v0.2.0
```

Artifacts:

```text
Jinja2Tools-0.2.0-win64.zip
Jinja2Tools-0.2.0-win64.zip.sha256
```

## Manual run

Open **Actions > Release Build > Run workflow**. Enter a version. Disable publishing to create only a workflow artifact, or enable it to create a tag and release.

## Repository permissions

The workflow declares `contents: write`. If publishing is denied, open **Settings > Actions > General > Workflow permissions** and permit read/write access for workflows.

## Toolchains

GitHub uses `windows-2022` and explicitly configures `Visual Studio 17 2022`. This is intentionally independent of a local Visual Studio 18 preset.

## vcpkg

`vcpkg.json` must contain a valid 40-character `builtin-baseline`. Dependencies are restored in manifest mode.

## ZIP layout

```text
Jinja2Tools/
├── Jinja2Tools.dll
├── Jinja2Tools.Helper.exe
├── README.md
├── LICENSE
└── THIRD_PARTY_NOTICES.md
```
