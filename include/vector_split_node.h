#ifndef PIRANHA_VETOR_SPLIT_NODE_H
#define PIRANHA_VETOR_SPLIT_NODE_H

#include "../include/node.h"

#include "../include/vector_split.h"
#include "../include/fundamental_types.h"

#include <string>
#include <iostream>

namespace piranha {

	class VectorSplitNode : public Node {
	public:
		VectorSplitNode() {
			/* void */
		}

		~VectorSplitNode() {
			/* void */
		}

	protected:
		virtual void _initialize() {
			/* void */
		}

		virtual void _evaluate() {
			/* void */
		}

		virtual void registerInputs() {
			registerInput(&m_primaryInput, "__in", &FundamentalType::VectorType);
			registerInput(m_outputX.getInputConnection(), "__in", &FundamentalType::VectorType);
			registerInput(m_outputY.getInputConnection(), "__in", &FundamentalType::VectorType);
			registerInput(m_outputZ.getInputConnection(), "__in", &FundamentalType::VectorType);
			registerInput(m_outputW.getInputConnection(), "__in", &FundamentalType::VectorType);
		}

		virtual void registerOutputs() {
			setPrimaryOutputReference(&m_primaryInput);

			registerOutputReference(&m_primaryInput, "$primary");
			registerOutput(&m_outputX, "x");
			registerOutput(&m_outputY, "y");
			registerOutput(&m_outputZ, "z");
			registerOutput(&m_outputW, "w");
		}

	protected:
		pNodeInput m_primaryInput;

		VectorSplitOutput<0> m_outputX;
		VectorSplitOutput<1> m_outputY;
		VectorSplitOutput<2> m_outputZ;
		VectorSplitOutput<3> m_outputW;
	};

} /* namespace piranha */

#endif /* PIRANHA_VETOR_SPLIT_NODE_H */