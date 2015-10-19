#include "Texture.h"
#include "FileReader.h"
#include "Text.h"
#include "GM_AdvText.h"
#define pictunreumMAX	100

class CInterSystemDef
{
private:
	Ctexture		systemTexture;
	CFileReader		systemfr;
	Ctext			systemText;
	CADV			systemadv;
	int				fontnum;
	int	systempicturenum[pictunreumMAX];

	public:
		
	void inter_systeminit(Ctexture		copysystemTexture,CFileReader		copysystemfr,CADV copyadv);
	void inter_systemdraw();
};


