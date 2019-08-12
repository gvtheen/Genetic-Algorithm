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
#include<cmath>
#include <boost/algorithm/string.hpp>
#include <algorithm>
#include "CGpopulation.h"
#include "../Util/log.hpp"
#include "../IACS.h"

namespace GAZJUT{

CGpopulation::CGpopulation()
{
    m_pMinRawGenome  = nullptr;
    m_pMaxRawGenome  = nullptr;
   m_pObjGaparameter = nullptr;
    m_pMinFitGenome  = nullptr;
    m_pMaxFitGenome  = nullptr;
    m_pMaxOriGenome  = nullptr;
    m_pMaxOriGenome  = nullptr;
}
CGpopulation::CGpopulation(CGaparameter* my_Gaparameter)
{
	assert(my_Gaparameter);
    assert(my_Gaparameter->PopNum());

    //util::Log::Info<<"CGpopulation::CGpopulation(CGaparameter* my_Gaparameter)!\n";
	//firstly built empty vector<VarRangeStruct>
	this->m_pObjGaparameter=my_Gaparameter;

	for(size_t i=0;i<my_Gaparameter->PopNum();i++)
    {
        CGenome *it = new CGenome(m_pObjGaparameter->VarRange(),m_pObjGaparameter->CodeMode(),i);
        m_Gpopulation.push_back(it);
    }

    //until this time, both best genome and worst genome are unknown.
    m_pMinRawGenome  = nullptr;
    m_pMaxRawGenome  = nullptr;
    m_pMinFitGenome  = nullptr;
    m_pMaxFitGenome  = nullptr;
    m_pMaxOriGenome  = nullptr;
    m_pMaxOriGenome  = nullptr;
    m_pBestGenome = new CGenome(m_pObjGaparameter->VarRange(),m_pObjGaparameter->CodeMode(),my_Gaparameter->PopNum());
    //util::Log::Info<<"End CGpopulation::CGpopulation(CGaparameter* my_Gaparameter)!\n";
}
CGpopulation::CGpopulation(CGpopulation& mypopulation)
{
	//assert(mypopulation);
    assert(mypopulation.m_pObjGaparameter);
    //util::Log::Info<<"CGpopulation::CGpopulation(CGpopulation& mypopulation)!\n";
    // pointer array
    this->m_pObjGaparameter= mypopulation.m_pObjGaparameter;
	CGenome* tempCGenome=nullptr;
	for(size_t i=0;i<m_pObjGaparameter->PopNum();i++)
    {
        tempCGenome = new CGenome(mypopulation[i]);
        m_Gpopulation.push_back(tempCGenome);
    }
    //util::Log::Info<<"CGpopulation::CGpopulation(CGpopulation& mypopulation)-2!\n";
    //util::Log::Info<<mypopulation.m_pMinRawGenome->index()<<"  CGpopulation::CGpopulation(CGpopulation& mypopulation)!\n";
    this->m_pMinRawGenome=this->m_Gpopulation[mypopulation.m_pMinRawGenome->index()];

    //util::Log::Info<<mypopulation.m_pMaxRawGenome->index()<<"  CGpopulation::CGpopulation(CGpopulation& mypopulation)!\n";
    this->m_pMaxRawGenome=this->m_Gpopulation[mypopulation.m_pMaxRawGenome->index()];

    //util::Log::Info<<mypopulation.m_pMinFitGenome->index()<<"  CGpopulation::CGpopulation(CGpopulation& mypopulation)!\n";
    this->m_pMinFitGenome=this->m_Gpopulation[mypopulation.m_pMinFitGenome->index()];

    //util::Log::Info<<mypopulation.m_pMaxFitGenome->index()<<"  CGpopulation::CGpopulation(CGpopulation& mypopulation)!\n";
    this->m_pMaxFitGenome=this->m_Gpopulation[mypopulation.m_pMaxFitGenome->index()];

    //util::Log::Info<<mypopulation.m_pMinOriGenome->index()<<"  CGpopulation::CGpopulation(CGpopulation& mypopulation)!\n";
    this->m_pMinOriGenome=this->m_Gpopulation[mypopulation.m_pMinOriGenome->index()];

    //util::Log::Info<<mypopulation.m_pMaxOriGenome->index()<<"  CGpopulation::CGpopulation(CGpopulation& mypopulation)!\n";
    this->m_pMaxOriGenome=this->m_Gpopulation[mypopulation.m_pMaxOriGenome->index()];
    //util::Log::Info<<"2-CGpopulation::CGpopulation(CGpopulation& mypopulation)!\n";

    m_pBestGenome = new CGenome(m_pObjGaparameter->VarRange(),m_pObjGaparameter->CodeMode(),this->m_pObjGaparameter->PopNum());
    *(this->m_pBestGenome) = *(mypopulation.m_pBestGenome);
    //util::Log::Info<<"end CGpopulation::CGpopulation(CGpopulation& mypopulation)!\n";
}
CGpopulation::~CGpopulation()
{
    for(size_t i=0;i<m_Gpopulation.size();i++)
	    delete m_Gpopulation[i];
    m_Gpopulation.clear();

    if(this->m_pBestGenome!=nullptr)
         delete this->m_pBestGenome;
}
CGpopulation* CGpopulation::clone()
{
	return new CGpopulation(*this);
}
//operator function
CGenome& CGpopulation::operator[](const size_t index)
{
	if(index>=m_pObjGaparameter->PopNum())
    {
         util::Log::Error<<"Index is out of range! CGpopulation_operator[]!\n";
         boost::throw_exception(std::runtime_error("Index is out of range! CGpopulation_operator[]!"));
    }
	return *(m_Gpopulation[index]);
}
double CGpopulation::operator[](std::string mystr)
{
	 std::string indexName = boost::to_lower_copy<std::string>(mystr);

	 if (indexName=="minraw")
	     return this->m_MinRawScore;
	 else if(indexName=="minfit")
	     return this->m_MinFitness;
	 else if(indexName=="maxfit")
	     return this->m_MaxFitness;
	 else if(indexName=="maxraw")
	     return this->m_MaxRawScore;
	 else if(indexName=="avgraw")
	     return this->m_AvgRawScore;
	 else if(indexName=="avgfit")
	     return this->m_AvgFitness;
     else if(indexName=="rawdev")
         return this->m_DevrawScore;
     else if(indexName=="minori")
         return this->m_MinOriValue;
     else if(indexName=="maxori")
         return this->m_MaxOriValue;
	 else{
		 util::Log::Error<<"IndexStr cannot match with them! CGpopulation_operator[]!\n";
         boost::throw_exception(std::runtime_error("IndexStr cannot match with them! CGpopulation_operator[]!"));
	 }
}

// property function
//void CGpopulation::init()
//{
//
//}
void CGpopulation::ascendSort()
{
	std::sort(m_Gpopulation.begin(),m_Gpopulation.end(),\
      [](CGenome* A,CGenome* B){ return (*A)["fitness"] < (*B)["fitness"];});   // use lamba

    // after sorting, index of Genome must be reseted
    for(size_t i=0;i<m_Gpopulation.size();i++)
        m_Gpopulation[i]->setIndex(i);
}
void CGpopulation::descendSort()
{
	std::sort(m_Gpopulation.begin(),m_Gpopulation.end(),\
      [](CGenome* A,CGenome* B){ return (*A)["fitness"] > (*B)["fitness"];});   // use lamba

    // after sorting, index of Genome must be reseted.
    for(size_t i=0;i<m_Gpopulation.size();i++)
        m_Gpopulation[i]->setIndex(i);
}
//after genetic operatoration, updatepopulation is necessary.
void CGpopulation::updatePopulation()
{
//	std::vector <CGenome*>::iterator it;
//	for(it=this->m_Gpopulation.begin();it<this->m_Gpopulation.end();it++)
//		(*it)->updateDecValueGene();
}
void CGpopulation::fitness_statistic()
{
    assert(m_Gpopulation.size()>0);

    std::vector <CGenome*>::iterator it;
	double sumFit=0.0,tmp=0.0;

    for(it=m_Gpopulation.begin();it<m_Gpopulation.end();it++)
        sumFit = sumFit + (*it)->fitness();

    this->m_AvgFitness  = sumFit/this->popNum();

	for(it=m_Gpopulation.begin();it<m_Gpopulation.end();it++)
	{
		(*it)->setRelativefitness( (*it)->fitness() / sumFit );
		tmp = tmp + (*it)->relativefitness();
		(*it)->setCumufitness(tmp);
	}
	it = std::max_element(m_Gpopulation.begin(),m_Gpopulation.end(),\
                         [](CGenome* A,CGenome* B){ return (*A)["fitness"] < (*B)["fitness"];});

    this->m_pMaxFitGenome =*it;
    it = std::min_element(m_Gpopulation.begin(),m_Gpopulation.end(),\
                                    [](CGenome* A,CGenome* B){ return (*A)["fitness"] < (*B)["fitness"];});
    this->m_pMinFitGenome = *it;
    this->m_MinFitness = this->m_pMinFitGenome->fitness();
    this->m_MaxFitness = this->m_pMaxFitGenome->fitness();
}
void CGpopulation::raw_statistic()
{
	assert(m_Gpopulation.size()>0);

	std::vector <CGenome*>::iterator it;
    double sumRawScore=0.0,s,tmp=0;

	for(it=m_Gpopulation.begin();it<m_Gpopulation.end();it++)
		sumRawScore = sumRawScore +(*it)->rawscore();

    this->m_AvgRawScore = sumRawScore/this->popNum();

	for(it=m_Gpopulation.begin();it<m_Gpopulation.end();it++)
	{
		s=(*it)->rawscore() - this->m_AvgRawScore;
		tmp = tmp + s*s;
	}
    this->m_DevrawScore = sqrt(tmp);

    it = std::max_element(m_Gpopulation.begin(),m_Gpopulation.end(),\
                           [](CGenome* A,CGenome* B){ return (*A)["rawscore"] < (*B)["rawscore"];});
    this->m_pMaxRawGenome = *it;

    it = std::min_element(m_Gpopulation.begin(),m_Gpopulation.end(),\
                                    [](CGenome* A,CGenome* B){ return (*A)["rawscore"] < (*B)["rawscore"];});
    this->m_pMinRawGenome = *it;

    this->m_MinRawScore = this->m_pMinRawGenome->rawscore();
    this->m_MaxRawScore = this->m_pMaxRawGenome->rawscore();
	//obtained best fitness
}
void CGpopulation::ori_statistic()
{
    assert(m_Gpopulation.size()>0);

    size_t current_generation = this->m_pObjGaparameter->Curr_Generation;

	std::vector <CGenome*>::iterator it;

	it = std::min_element(m_Gpopulation.begin(),m_Gpopulation.end(),\
                          [](CGenome* A,CGenome* B){ return (*A)["origvalue"] < (*B)["origvalue"];});
    this->m_pMinOriGenome = *it;

    it = std::max_element(m_Gpopulation.begin(),m_Gpopulation.end(),\
                          [](CGenome* A,CGenome* B){ return (*A)["origvalue"] < (*B)["origvalue"];});
    this->m_pMaxOriGenome = *it;

    this->m_MinOriValue = this->m_pMinOriGenome->origValue();
    this->m_MaxOriValue = this->m_pMaxOriGenome->origValue();

     if(current_generation==0)
       *m_pBestGenome= *m_pMinOriGenome;
     else if((*m_pBestGenome)["origvalue"]>this->m_MinOriValue)
       *m_pBestGenome= *m_pMinOriGenome;

}
//input output
size_t CGpopulation::popNum()
{
	return m_pObjGaparameter->PopNum();
}
void CGpopulation::setPopNum(size_t popnum)
{
    if(popnum<=0)
    {
        util::Log::Error<<"Population num is less than 0! CGpopulation_setPopNum!\n";
        boost::throw_exception(std::runtime_error("Population num is less than 0! CGpopulation_setPopNum!\n"));
    }
    m_pObjGaparameter->setKeyValue("Population",std::to_string(popnum));

}
std::vector <VarRangeStruct>* CGpopulation::VarRangeStructArray()
{
    return m_pObjGaparameter->VarRange();
}
void CGpopulation::setVarRangeStructArray(std::vector <VarRangeStruct>& my_VarRangeStruct)
{
    if(&my_VarRangeStruct==nullptr)
    {
        util::Log::Error<<"Pointer of my_VarRangeStruct is null! CGpopulation_setVarRangeStructArray!\n";
        boost::throw_exception(std::runtime_error("Pointer of my_VarRangeStruct is null! CGpopulation_setVarRangeStructArray!\n"));
    }
    m_pObjGaparameter->setVarRange(my_VarRangeStruct);
}
void CGpopulation::last_output()
{
   util::Log::Info<<"Best value:"<<this->m_pBestGenome->origValue()<<std::endl;
}
void CGpopulation::output()
{
    for(size_t i=0;i<this->popNum();i++){
        for(size_t j=0;j<(*m_Gpopulation[i]).geneNum();j++)
            util::Log::Info<<(*m_Gpopulation[i])[j]<<"  ";
        util::Log::Info<<std::endl;
        util::Log::Info<<"Value: "<<(*m_Gpopulation[i])["origvalue"]<<std::endl;
    }
    util::Log::Info<<std::endl;
    util::Log::Info<<"Until now, best population:"<<std::endl;
    for(size_t j=0;j<(*m_pBestGenome).geneNum();j++)
            util::Log::Info<<(*m_pBestGenome)[j]<<"  ";
        util::Log::Info<<std::endl;
        util::Log::Info<<"Value: "<<(*m_pBestGenome)["origvalue"]<<std::endl;
    util::Log::Info<<std::endl;
}

}


