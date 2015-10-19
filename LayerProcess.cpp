#include <stdlib.h>
#include "Common.h"



//レイヤー管理ID発行関数
int IssueLayerID(int layernum,_layerid *layerid)
{
	int issue;
	issue=(rand() % 9999)+1000;
	layerid->checker=issue;
	issue*=1000;
	issue+=layernum;
	layerid->layernum=layernum;
	return issue;
}


//レイヤー管理ID読み込み関数
_layerid ReadLayerID(int layeridnum)
{
	_layerid layerid;
	float tempA;
	int tempB;

	tempA=layeridnum/1000;
	tempB=(int)tempA;
	layerid.checker=tempB;
	tempB*=1000;
	
	layerid.layernum=(layeridnum-tempB);
	

	return layerid;
}
