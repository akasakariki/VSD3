#include <stdlib.h>
#include "Common.h"



//���C���[�Ǘ�ID���s�֐�
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


//���C���[�Ǘ�ID�ǂݍ��݊֐�
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
