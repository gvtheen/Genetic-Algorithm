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
#include "CGAEngine.h"
#include "CSelector.h"
#include "CEvaluator.h"
#include "CCross.h"
#include "CElist.h"
#include "CMutator.h"
#include "CEvaluator.h"
#include "CFitnessScaling.h"
#include "CGaOperatorBase.h"
#include "CGaparameter.h"
//#include "../CalcZJUT/CParameter.h"
//#include "CGpopulation.h"
//#include "../CalcZJUT/CExeVASP.h"
//#include "../CalcZJUT/CExeGaussian.h"
//#include "../CalcZJUT/CExeDMol.h"
//#include "../CalcZJUT/CExeLammps.h"
//#include "../CalcZJUT/CExeDFTB.h"
//#include "../CalcZJUT/CExeCastep.h"
//#include "../CalcZJUT/CStructPoolBase.h"
//#include "../CalcZJUT/CStructPoolCluster.h"
//#include "../CalcZJUT/CStructPoolSupported.h"
#include "../Util/log.hpp"
#include "../IACS.h"
using util::Log;
using CALCZJUT::CParameter;

namespace GAZJUT{

CGAEngine::CGAEngine(CGaparameter* para,EVALUATOR_FUN _fun)
{
    m_pGaparameter=para;
    m_evaluator_fun=_fun;
}

CGAEngine::~CGAEngine()
{
    delete m_pCurrentPopulation;
    //delete this->m_pStructurePool;
    for(size_t i=0;i<m_GeneticOperator.size();i++)
          delete m_GeneticOperator[i];
    m_GeneticOperator.clear();
}
void CGAEngine::init()
{
   std::cout << "Initialize world-1!" << std::endl;
   Log::Info<<"Initialize GA Engine....."<< std::endl;

   m_pCurrentPopulation = new CGpopulation(m_pGaparameter);

   m_GeneticOperator.push_back(new CEvaluator(m_evaluator_fun));
   m_GeneticOperator.push_back(new CFitnessScaling());
   m_GeneticOperator.push_back(new CElist());
   m_GeneticOperator.push_back(new CSelector());
   m_GeneticOperator.push_back(new CCross());
   m_GeneticOperator.push_back(new CMutator());

   Log::Info<<"End Initialize GA Engine....."<< std::endl;

}
void CGAEngine::evolve()
{
   // read the setting generation number
   Log::Info<<"Enter evolving process of GA Engine....."<< std::endl;

   size_t total_pop_num=m_pGaparameter->GenerationNum();

   while(true){

       //std::cout << "evolve: " <<m_pGaparameter->Curr_Generation<<std::endl;
       for(size_t i=0;i<m_GeneticOperator.size();i++){
          m_GeneticOperator[i]->run(m_pCurrentPopulation);
          if(i==0)
             m_pCurrentPopulation->output();
       }

       m_pGaparameter->add_Curr_Generation();

       if(m_pGaparameter->Curr_Generation >= total_pop_num)
          break;
   }

   Log::Info<<"End evolving process of GA Engine"<< std::endl;
   this->m_pCurrentPopulation->last_output();
}

}
