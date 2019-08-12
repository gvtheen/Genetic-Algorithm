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
#include "CSelector.h"
#include "CGaparameter.h"
#include "CGenome.h"
#include "GaDeclaration.h"
#include "../Util/log.hpp"
#include "../IACS.h"
#include "../Util/CRandomgenerator.h"
using util::Log;
using namespace GAZJUT;

namespace GAZJUT{

//constructor
CSelector::CSelector()
:CGaOperatorBase()
{
    this->mix_index=0;
}
CSelector::CSelector(const CSelector& myselector)
{
    //this->CSelector();
}
//deconstructor
CSelector::~CSelector()
{
}
//property function
CSelector* CSelector::clone()
{
	return new CSelector(*this);
}

void CSelector::run(CGpopulation* PCurrentPopulation)
{
	assert(PCurrentPopulation);
    //util::Log::Info<<"CSelector::run!\n";
    if(PCurrentPopulation==nullptr)
    {
        Log::Error<<"Error: CurrentPopulation is null! CSelector_run!\n";
        boost::throw_exception(std::runtime_error("Error: CurrentPopulation is null! CSelector_run!\n"));
    }
    // run statistic calculation
	PCurrentPopulation->fitness_statistic();
	//dispatch selector
	switch ((int)(PCurrentPopulation->m_pObjGaparameter->SearchType()))
	{
		case ROULETTE_WHEEL:
		    this->roulette_Wheel_Select(PCurrentPopulation);
		    break;
		case RANDOM:
			this->rondom_Select(PCurrentPopulation);
			break;
		case TOURNAMENT:
			this->tournament_Select(PCurrentPopulation);
			break;
		case MIXED:
		    this->mix_index +=1;
		    if(mix_index%3==0)
               this->roulette_Wheel_Select(PCurrentPopulation);
            else if(mix_index%3==1)
               this->rondom_Select(PCurrentPopulation);
            else if(mix_index%3==2){
               this->tournament_Select(PCurrentPopulation);
               this->mix_index=2;
            }
			break;
		default:
			this->roulette_Wheel_Select(PCurrentPopulation);
		    break;
	}
	//util::Log::Info<<"end CSelector::run!\n";
}

void CSelector::roulette_Wheel_Select(CGpopulation* P_CurrentPopulation)
{
	// copy original population to temp population for further treatment.
	// After selector process, current population (m_PCurrentPopulation) modified is new population
    //util::Log::Info<<"CSelector::roulette_Wheel_Select!\n";
	CGpopulation* P_copy_of_CurrentPopulation = P_CurrentPopulation->clone();
	std::vector <CGenome*>& R_copy_currentPopulation = P_copy_of_CurrentPopulation->m_Gpopulation;

	std::vector <CGenome*>::iterator it;
	double rndNum;
    util::CRandomgenerator *Rndgenerator=new util::CRandomgenerator();
    unsigned int seedNum = 0;

    std::vector<double> tempDoubleVect;

    std::vector<size_t> copy_label(P_CurrentPopulation->popNum());

	for (it=P_CurrentPopulation->m_Gpopulation.begin();it<P_CurrentPopulation->m_Gpopulation.end();it++)
	{
		rndNum=Rndgenerator->uniformRandom01(seedNum++);
		if( rndNum < R_copy_currentPopulation[0]->cumufitness()){
            // Get Dec value from copy population
            if(copy_label[0] < P_CurrentPopulation->m_pObjGaparameter->SelectCUTOFFNum ){
              R_copy_currentPopulation[0]->getDecValue(tempDoubleVect);
            // Set Dec value into current Population
              (*it)->updateDecValueGene(tempDoubleVect);
              copy_label[0] = copy_label[0]+1;
            }else
              (*it)->init();
        }else{
			for(size_t i=0; i < R_copy_currentPopulation.size() - 1; i++){
                if(rndNum > R_copy_currentPopulation[i]->cumufitness() &&  \
                    rndNum < R_copy_currentPopulation[i+1]->cumufitness()){
                        // Get Dec value from copy population
                    if(copy_label[i] < P_CurrentPopulation->m_pObjGaparameter->SelectCUTOFFNum ){
                        R_copy_currentPopulation[i]->getDecValue(tempDoubleVect);
                        // Set Dec value into current Population
                       (*it)->updateDecValueGene(tempDoubleVect);
                        copy_label[i] = copy_label[i]+1;
                    }else
                       (*it)->init();
                }
			}
		}
	}
	delete Rndgenerator;
	delete P_copy_of_CurrentPopulation;
	//util::Log::Info<<"End CSelector::roulette_Wheel_Select!\n";
}
void CSelector::rondom_Select(CGpopulation* P_CurrentPopulation)
{
	// copy original population to temp population for further treatment.
	// After selector process, current population (m_PCurrentPopulation) modified is new population
	//util::Log::Info<<"CSelector::rondom_Select!\n";
	CGpopulation* P_copy_of_currentPopulation = P_CurrentPopulation->clone();

	double pointerDistance;
	int pointer;
    util::CRandomgenerator *Rndgenerator=new util::CRandomgenerator();

	pointerDistance=1.0/P_copy_of_currentPopulation->popNum();
	pointer=pointerDistance*Rndgenerator->uniformRandom01(100);

	std::vector<double> tempDoubleVect;
	std::vector<size_t> copy_label(P_CurrentPopulation->popNum());

	for (size_t i=0;i<P_CurrentPopulation->popNum();i++)
	{
		pointer=pointer+i*pointerDistance;
		if(pointer < (*P_copy_of_currentPopulation)[0].cumufitness()){
             if(copy_label[0] < P_CurrentPopulation->m_pObjGaparameter->SelectCUTOFFNum ){
                 (*P_copy_of_currentPopulation)[0].getDecValue(tempDoubleVect);
                 (*P_CurrentPopulation)[i].updateDecValueGene(tempDoubleVect);
                 copy_label[0] = copy_label[0]+1;
             }else
                 (*P_CurrentPopulation)[i].init();
		}else{
			for(size_t j=0;j<P_copy_of_currentPopulation->popNum() - 1;j++){
               if(pointer > (*P_copy_of_currentPopulation)[j].cumufitness() && \
				   pointer < (*P_copy_of_currentPopulation)[j+1].cumufitness() ){
				      if(copy_label[j] < P_CurrentPopulation->m_pObjGaparameter->SelectCUTOFFNum ){
				         (*P_copy_of_currentPopulation)[j].getDecValue(tempDoubleVect);
				         (*P_CurrentPopulation)[i].updateDecValueGene(tempDoubleVect);
				          copy_label[j] = copy_label[j]+1;
				      }else
                          (*P_CurrentPopulation)[i].init();
               }
			}
		}
	}
	//util::Log::Info<<"End_CSelector::rondom_Select!\n";
	delete P_copy_of_currentPopulation;
	delete Rndgenerator;

}
void CSelector::tournament_Select(CGpopulation* P_CurrentPopulation)
{
//util::Log::Info<<"CSelector::tournament_Select!\n";

	CGpopulation* P_copy_of_currentPopulation = P_CurrentPopulation->clone();
	//std::vector <CGenome*>& R_p_copy_currentPopulation = P_copy_of_currentPopulation->m_Gpopulation;

	int p_n1,p_n2;
    util::CRandomgenerator *Rndgenerator=new util::CRandomgenerator();
    unsigned int seedNum=1;
    size_t popNum=P_CurrentPopulation->popNum();

    std::vector<double> tempDoubleVect;
    std::vector<size_t> copy_label(P_CurrentPopulation->popNum());

	for(size_t i=0;i<popNum;i++)
	{
		p_n1=(int)(popNum*(Rndgenerator->uniformRandom01(seedNum++)));
		p_n2=(int)(popNum*(Rndgenerator->uniformRandom01(seedNum++)));

		// comparing value is fitness, but not cumufitness;
		if((*P_copy_of_currentPopulation)[p_n1].fitness() > (*P_copy_of_currentPopulation)[p_n2].fitness()){
            if(copy_label[p_n1] < P_CurrentPopulation->m_pObjGaparameter->SelectCUTOFFNum ){
              (*P_copy_of_currentPopulation)[p_n1].getDecValue(tempDoubleVect);
              (*P_CurrentPopulation)[i].updateDecValueGene(tempDoubleVect);
              copy_label[p_n1] = copy_label[p_n1]+1;
            }else
               (*P_CurrentPopulation)[i].init();
		}else{
		    if(copy_label[p_n2] < P_CurrentPopulation->m_pObjGaparameter->SelectCUTOFFNum ){
		      (*P_copy_of_currentPopulation)[p_n2].getDecValue(tempDoubleVect);
              (*P_CurrentPopulation)[i].updateDecValueGene(tempDoubleVect);
               copy_label[p_n2] = copy_label[p_n2]+1;
		    }else
		       (*P_CurrentPopulation)[i].init();
		}
	}
	//util::Log::Info<<"End_CSelector::tournament_Select!\n";
	delete P_copy_of_currentPopulation;
	delete Rndgenerator;
}



}///namespace
