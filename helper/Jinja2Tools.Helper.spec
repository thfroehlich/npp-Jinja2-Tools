from PyInstaller.utils.hooks import collect_submodules
hiddenimports=collect_submodules("jinja2")
a=Analysis(["src/jinja2_tools_helper/__main__.py"],pathex=["src"],hiddenimports=hiddenimports)
pyz=PYZ(a.pure)
exe=EXE(pyz,a.scripts,a.binaries,a.datas,[],name="Jinja2Tools.Helper",console=False)
