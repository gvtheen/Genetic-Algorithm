/******************************************************************************
**
** Copyright (C) 2019-2031 Dr.Gui-lin Zhuang <glzhuang@zjut.edu.cn>
** All rights reserved.
**
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
******************************************************************************/
#include <float.h>
#include <cmath>
#include "../Util/log.hpp"
#include "CEvaluator.h"
#include "CGpopulation.h"
#include "CGenome.h"
/* #include "../CalcZJUT/CExeFitnessInterface.h"
 * #include "../CalcZJUT/CStructPoolBase.h"
 * #include "../CalcZJUT/CModelBase.h"
 * #include "../CalcZJUT/CParameter.h"
 */
#include "../Util/Bitset.h"
#include "../Util/log.hpp"


using util::Bitset;
using util::Log;

namespace CALCZJUT{
   class CStructPoolBase;
   class CParameter;
}

namespace GAZJUT{

CEvaluator::CEvaluator(EVALUATOR_FUN _fun)
:m_evaluator(_fun)
{

}
/* CEvaluator::CEvaluator(std::vector<CALCZJUT::CExeFitnessInterface*>  myEvaluatorPool,
 *                        CALCZJUT::CStructPoolBase *myStructPool)
 * {
 *     // sample evaluator
 *     this->m_pEvaluatorPool.assign(myEvaluatorPool.begin(),myEvaluatorPool.end());
 *     this->m_currentEvaluator=nullptr;
 *     // sample structural pool
 *     this->m_pStructurePool = myStructPool;
 *     // sample evaluator monitor!
 *     pop_run_state.resize(m_pStructurePool->m_pParameter->popNum(),false);
 * }
 */
CEvaluator::~CEvaluator()
{

}
void CEvaluator::run(CGpopulation* CurrentPopulation)
{
    //util::Log::Info<<"CEvaluator::run!\n";
    std::vector<double> temp;
    for(size_t i=0;i<CurrentPopulation->popNum();i++){
        (*CurrentPopulation)[i].getDecValue(temp);
        (*CurrentPopulation)[i].setOrigValue((*m_evaluator)(temp));
    }

    CurrentPopulation->ori_statistic();
    //util::Log::Info<<"End CEvaluator::run!\n";
}
}
