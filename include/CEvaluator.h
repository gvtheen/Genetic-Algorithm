#ifndef CEVALUATOR_H
#define CEVALUATOR_H
#include <vector>
#include <map>
#include <iostream>

#include "CGpopulation.h"
#include "CGaOperatorBase.h"
#include "../Util/Bitset.h"
#include "GaDeclaration.h"
/*
The main function of class is that FitnessValuator conducts calculation to
 get originalValue and RawScore.

*/

using util::Bitset;

/* namespace CALCZJUT{
 *   class CExeFitnessInterface;
 *   class CModelBase;
 *   class CStructPoolBase;
 *   class CParameter;
 * }
 */
namespace GAZJUT{

class CEvaluator:public CGaOperatorBase
{
	public:
	    typedef enum{OPTIMAL_ENERGY = 0x9231A,
                        OPTIMAL_GAP = 0x9232A
                    }OPT_TYPE;

		CEvaluator(EVALUATOR_FUN);
//		CEvaluator(std::vector<CALCZJUT::CExeFitnessInterface*>,
//                   CALCZJUT::CStructPoolBase*);
		~CEvaluator();

		void clone();
		void init();
		void run( CGpopulation* );

	protected:

    private:
           EVALUATOR_FUN  m_evaluator;

};

}
#endif
