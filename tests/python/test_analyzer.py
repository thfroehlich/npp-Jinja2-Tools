from jinja2 import Environment
from jinja2_tools_helper.analyzer import analyze_template
def test_analysis():
 r=analyze_template(Environment(),"{% macro m(x) %}{{ x|upper }}{% endmacro %}{% include 'a.j2' %}","x");assert r.macros==["m"] and r.filters==["upper"] and r.dependencies==["a.j2"]
def test_dynamic():assert analyze_template(Environment(),"{% include target %}","x").dynamic_dependencies
