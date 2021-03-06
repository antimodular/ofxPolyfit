/*
 *  powerBases.cpp
 *  polyncpp-test
 *
 *  Created by Elliot Woods on 26/03/2010.
 *  Copyright 2010 Kimchi and Chips. All rights reserved.
 *
 */

#include "powerBases.h"

powerBases::powerBases(pfitBasisType basesShape)
{
	_basesShape = basesShape;
    _renderedNDimensions=0;
    _renderedNPowerOrder=0;
}

int powerBases::updatebasisIndices(int nDimensions, int nPowerOrder)
{
	_nDimensions = nDimensions;
	_nPowerOrder = nPowerOrder;
	
	if (_nDimensions != _renderedNDimensions
		|| _nPowerOrder != _renderedNPowerOrder)
	{
		calcBases();
		_renderedNDimensions = _nDimensions;
		_renderedNPowerOrder = _nPowerOrder;
	}
	
	return _nBases;
}

void powerBases::calcBases()
{

	if (_basesShape < BASIS_SHAPE_PADE_FIRST)
	{
		// create temporary variables for each item
		int *iPossibleItemX = new int[_nDimensions];
		bool present;
		
		//
		// clean out bases
		//
		clearBases();
		
		//
		// iterate through all possible items for square/cubic/etc
		//
		for (int iPossibleItem=0; iPossibleItem < pow(_nPowerOrder+1,(double)_nDimensions); iPossibleItem++)
		{
			
			//
			// calculate Indices in each dimension for this item
			//
			int iOrderSum=0;
			
			for (int iDimension=0; iDimension < _nDimensions; iDimension++)
			{
				iPossibleItemX[iDimension] = iPossibleItem 
				/ (int)pow(_nPowerOrder+1,(double)iDimension)
				% (_nPowerOrder+1);
				
				if (iDimension<_nDimensions-1 || _basesShape==BASIS_SHAPE_SQUARE_MINUS_HIGHEST)
					iOrderSum += iPossibleItemX[iDimension];
			}
			
			
			//
			// determine whether current index should be selected
			//
			switch(_basesShape)
			{
			case BASIS_SHAPE_TRIANGLE:
				present = iOrderSum < (_nPowerOrder+1) - iPossibleItemX[_nDimensions-1];
				break;

			case BASIS_SHAPE_SQUARE:
				present = true;
				break;

			case BASIS_SHAPE_SQUARE_MINUS_HIGHEST:
				present = iOrderSum < _nDimensions;
				break;
			}
			
			
			//
			// Fill array with data Indices
			//
			
			int idxbasisIndices;
			if (present)
			{
				vecBasisIndices.push_back(new unsigned int[_nDimensions]);
				idxbasisIndices = (int) vecBasisIndices.size()-1;
				
				for (int iDimension=0; iDimension<_nDimensions; iDimension++)
				{
					vecBasisIndices.at(idxbasisIndices)[iDimension]=iPossibleItemX[iDimension];
					
				}
			}		
		}
		
		_nBases = (unsigned int) vecBasisIndices.size();
		
		_renderedNDimensions = _nDimensions;
		_renderedNPowerOrder = _nPowerOrder;

		delete[] iPossibleItemX;

	}  else if (_basesShape == BASIS_SHAPE_PADE_FIRST )  {

		//
		// PADE BASES
		//
        
        // Input X Y Z X'
        // Output X' Y'

		_nDimensions=4;
		vecBasisIndices.clear();
		for (int iBasis=0; iBasis<7; iBasis++)
		{
			vecBasisIndices.push_back(new unsigned int[_nDimensions]);
			for (int iDimension=0; iDimension<_nDimensions; iDimension++)
				vecBasisIndices[iBasis][iDimension]=0;
		}

		//x x'
		vecBasisIndices[0][0]=1;
		vecBasisIndices[0][3]=1;

		//y x'
		vecBasisIndices[1][1]=1;
		vecBasisIndices[1][3]=1;

		//z x'
		vecBasisIndices[2][2]=1;
		vecBasisIndices[2][3]=1;

		//x
		vecBasisIndices[3][0]=1;

		//y
		vecBasisIndices[4][1]=1;

		//z
		vecBasisIndices[5][2]=1;

		//constant
		//

		_nBases = (unsigned int) vecBasisIndices.size();
		
		_renderedNDimensions = _nDimensions;
		_renderedNPowerOrder = _nPowerOrder;
        
	} else if (_basesShape == BASIS_SHAPE_PADE_SECOND ) {
        
        //
        // PADE BASES 2nd order
        // 14 coefficients
        //
        
        _nDimensions=4;
        vecBasisIndices.clear();
        for (int iBasis=0; iBasis<14; iBasis++)
        {
            vecBasisIndices.push_back(new unsigned int[_nDimensions]);
            for (int iDimension=0; iDimension<_nDimensions; iDimension++)
                vecBasisIndices[iBasis][iDimension]=0;
        }
        
        //0 A x
        vecBasisIndices[0][0]=1;
        
        //1 B y
        vecBasisIndices[1][1]=1;
        
        //2 C z
        vecBasisIndices[2][2]=1;
        
        //3 D x x
        vecBasisIndices[3][0]=2;
        
        //4 E y y
        vecBasisIndices[4][1]=2;
        
        //5 F z z
        vecBasisIndices[5][2]=2;
        
        //6 G x y
        vecBasisIndices[6][0]=1;
        vecBasisIndices[6][1]=1;
        
        //7 H y z
        vecBasisIndices[7][1]=1;
        vecBasisIndices[7][2]=1;
        
        //8 I z x
        vecBasisIndices[8][0]=1;
        vecBasisIndices[8][2]=1;
        
        //9 J x y z
        vecBasisIndices[9][0]=1;
        vecBasisIndices[9][1]=1;
        vecBasisIndices[9][2]=1;
        
        
        //10 K - 
        //constant
        //
        
        //11 L x x'
        vecBasisIndices[11][0]=1;
        vecBasisIndices[11][3]=1;
        
        //12 M y x'
        vecBasisIndices[12][1]=1;
        vecBasisIndices[12][3]=1;
        
        //13 N z x'
        vecBasisIndices[13][2]=1;
        vecBasisIndices[13][3]=1;
        
        _nBases = (unsigned int) vecBasisIndices.size();
        
        _renderedNDimensions = _nDimensions;
        _renderedNPowerOrder = _nPowerOrder;
    }
}

void powerBases::clearBases()
{
	for (int iBasis=0; iBasis < vecBasisIndices.size(); iBasis++)
		delete[] vecBasisIndices[iBasis];

	vecBasisIndices.clear();
	_nBases=0;
}