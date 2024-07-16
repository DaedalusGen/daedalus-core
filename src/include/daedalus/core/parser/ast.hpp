#ifndef __DAEDALUS_CORE_AST__
#define __DAEDALUS_CORE_AST__

#include <memory>
#include <string>
#include <vector>

namespace daedalus {
    namespace core {
        namespace ast {

    		class Statement;
    		class Scope;
    		class Expression;
    		class NumberExpression;

    		/**
    		 * Statement
    		 * @note This class is only used for inheritance purpose, never as a value
    		 */
    		class Statement {
    		public:

    			/**
    			 * Get the NodeType of the Statement
    			 */
    			virtual std::string type();

    			/**
    			 * Get the string representation of the Statement
    			 */
    			virtual std::string repr(int indent = 0);
    		};

    		/**
    		 * Expression < Statement
    		 * @note This class is only used for inheritance purpose, never as a value
    		 */
    		class Expression : public Statement, public std::enable_shared_from_this<Expression> {
    		public:
    			/**
    			 * Get the constexpr version of the node (can be evaluated by parser)
    			 */
    			virtual std::shared_ptr<Expression> get_constexpr();
    		};

            /**
    		 * Scope < Statement
    		 * @note This class is only used for inheritance purpose or as a general wrapper for a program
    		 */
    		class Scope : public Expression {
    		public:
    			std::vector<std::shared_ptr<Expression>> body;

    			Scope(std::vector<std::shared_ptr<Expression>> body = std::vector<std::shared_ptr<Expression>>());

    			virtual std::string type() override;
                virtual std::shared_ptr<Expression> get_constexpr() override;
    			virtual std::string repr(int indent = 0) override;
    		};

    		/**
    		 * NumberExpression < Expression < Statement
    		 */
    		class NumberExpression : public Expression {
    		public:
    			double value;

    			NumberExpression(double value);

    			virtual std::string type() override;
    			virtual std::shared_ptr<Expression> get_constexpr() override;
    			virtual std::string repr(int indent = 0) override;
    		};
    	}
    }
}

#endif // __DAEDALUS_CORE_AST__
