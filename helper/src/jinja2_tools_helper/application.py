from jinja2 import Environment,TemplateAssertionError,TemplateSyntaxError
from .analyzer import analyze_template
from .diagnostics import Diagnostic
from .formatter import format_template
from .protocol import Request,Response
def handle_request(r:Request)->Response:
 try:
  env=Environment();ast=env.parse(r.text,name=r.name,filename=r.name)
  if r.operation=="ping":result={"pong":True}
  elif r.operation=="validate":result={"valid":True}
  elif r.operation=="format":result={"formatted":format_template(r.text)}
  elif r.operation=="analyze":result=analyze_template(env,r.text,r.name).to_dict()
  else:result={"shutdown":True}
  return Response(r.request_id,True,[],result)
 except (TemplateSyntaxError,TemplateAssertionError) as e:return Response(r.request_id,False,[Diagnostic("error","JINJA_SYNTAX_ERROR",str(e),"jinja2",getattr(e,"lineno",1) or 1,1,"estimated",type(e).__name__)])
 except Exception as e:return Response(r.request_id,False,[Diagnostic("error","INTERNAL_ERROR",str(e),"helper",exceptionType=type(e).__name__)])
