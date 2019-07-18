#ifndef PIRANHA_VECTOR_CONSTRUCTOR_H
#define PIRANHA_VECTOR_CONSTRUCTOR_H

#include "pipe_node.h"
#include "vector_output.h"

namespace piranha {

	class VectorConstructorOutput : public VectorValueOutput {
	public:
		VectorConstructorOutput();
		~VectorConstructorOutput();

		virtual void fullCompute(void *target) const;
		virtual void registerInputs();

		pNodeInput *getXInputConnection() { return &m_inputX; }
		pNodeInput *getYInputConnection() { return &m_inputY; }
		pNodeInput *getZInputConnection() { return &m_inputZ; }
		pNodeInput *getWInputConnection() { return &m_inputW; }

	protected:
		pNodeInput m_inputX;
		pNodeInput m_inputY;
		pNodeInput m_inputZ;
		pNodeInput m_inputW;
	};

	class VectorConstructorNode : public Node {
	public:
		VectorConstructorNode() {
			/* void */
		}

		~VectorConstructorNode() {
			/* void */
		}

		virtual void _initialize() {
			m_output.initialize();
		}

		virtual void _evaluate() {
			/* void */
		}

		virtual void _destroy() {
			/* void */
		}

		virtual void registerOutputs() {
			setPrimaryOutput(&m_output);
			registerOutput(&m_output, "$primary");

			registerOutputReference(m_output.getXInputConnection(), "x");
			registerOutputReference(m_output.getYInputConnection(), "y");
			registerOutputReference(m_output.getZInputConnection(), "z");
			registerOutputReference(m_output.getWInputConnection(), "w");
		}

		virtual void registerInputs() {
			int inputCount = m_output.getInputCount();
			for (int i = 0; i < inputCount; i++) {
				// Generate a standard name
				std::stringstream ss;
				if (inputCount > 1) ss << "__in" << i;
				else ss << "__in";

				// Register all inputs of the one node output
				registerInput(m_output.NodeOutput::getInputConnection(i), ss.str(), &FundamentalType::FloatType);
			}
		}

	protected:
		VectorConstructorOutput m_output;
	};

} /* namespace piranha */

#endif /* PIRANHA_VECTOR_CONSTRUCTOR_H */
