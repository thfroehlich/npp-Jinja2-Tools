from dataclasses import dataclass, asdict

from jinja2 import Environment, meta, nodes


@dataclass(frozen=True)
class AnalysisResult:
    undeclared_variables: list[str]
    blocks: list[str]
    macros: list[str]
    filters: list[str]
    tests: list[str]
    dependencies: list[str]
    dynamic_dependencies: bool

    def to_dict(self) -> dict:
        return asdict(self)


def analyze_template(environment: Environment, source: str, name: str) -> AnalysisResult:
    ast = environment.parse(source, name=name, filename=name)
    references = list(meta.find_referenced_templates(ast) or [])
    return AnalysisResult(
        undeclared_variables=sorted(meta.find_undeclared_variables(ast)),
        blocks=sorted({node.name for node in ast.find_all(nodes.Block)}),
        macros=sorted({node.name for node in ast.find_all(nodes.Macro)}),
        filters=sorted({node.name for node in ast.find_all(nodes.Filter)}),
        tests=sorted({node.name for node in ast.find_all(nodes.Test)}),
        dependencies=sorted({value for value in references if value is not None}),
        dynamic_dependencies=any(value is None for value in references),
    )
