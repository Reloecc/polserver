#ifndef POLSERVER_FUNCTIONPARAMETERLIST_H
#define POLSERVER_FUNCTIONPARAMETERLIST_H

#include "bscript/compiler/ast/Node.h"

namespace Pol::Bscript::Compiler
{
class FunctionParameterDeclaration;

class FunctionParameterList : public Node
{
public:
  FunctionParameterList( const SourceLocation&,
                         std::vector<std::unique_ptr<FunctionParameterDeclaration>> parameters );

  void accept( NodeVisitor& visitor ) override;
  void describe_to( std::string& ) const override;
};

}  // namespace Pol::Bscript::Compiler


#endif  // POLSERVER_FUNCTIONPARAMETERLIST_H
