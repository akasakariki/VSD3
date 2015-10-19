
#include "Renderer.h"
#include "Scene.h"
#include "Texture.h"
#include "parser.h"
int reset_flag=0;

int adminnum;
int adminnum2;
int chars[3];
int soundtest;
int kk;
int test_blend=0;
int test_blend_f=1;
CFileReader fr;
//�^�O�e�X�g
CTag	tag;
_Tag	tags;
int s;
//���W���[���e�X�g�p(�X�N���v�g)
/*
int main()
{

	char scriptbody[10][255]=
	{
		"<tagtest test1=145 test22=12465    test67=fkakfsl>",
		"<tagtest test1=145  test22=12465 test67=fkakfsl>",
		"<tagtest   test1=145 test22=12465 test67=fkakfsl>",
		"<tagtest test1=145 test22=12465 test67=fkakfsl test67=fkakfsl test67=fkakfsl test67=fkakfsl test67=fkakfsl test67=fkakfsl test67=fkakfsl>",
		"end"
	};
	tag.Init_Tag(&tags);
	tag.push_parser(scriptbody[0],&tags);
	tag.push_parser(scriptbody[1],&tags);
	tag.push_parser(scriptbody[2],&tags);
	tag.push_parser(scriptbody[3],&tags);
	printf("�V�^�O��̓V�X�e��V1");
	tag.debugtest_pop_parser(&tags);//�S�o��


	scanf("%d",s);
	scanf("%d",s);
	return 0;
}
*/
int Renderer::Mainroutine()
{
	
	if(reset_flag==0)
	{

		//����������э쐬�֐�
		texture.AddressHWND(hwnd);
		adv_text.SettingText(&m_pD3DDevice);//�e�L�X�g���C���[�쐬
		adv_text.reset_font();
		
		texture.SettingTexture(m_pD3DDevice,m_pD3D);//�e�N�X�`���[���C���[�쐬
		texture.SetLoadImageColorKey(0,0,0);//�C���[�W�J���[�ݒ�W�W���j
		sound.DX_SoundStart();
		btext.SettingBlockText(&adv_text);
		variable.SettingVariable();

		//�ݒ�֌W
		adv.Set_textwindow(25,800,480);
		adv.format_text(&adv_text);

		//�f�o�b�O�p�ǂݍ���
		soundtest=sound.LoadSound("sound/Sample.wav");
		adminnum=adv_text.SetTextFont(25,0,3,false,false,false,"���C���I");

		btext.SetBlockText(adminnum,50,100,255,255,255,255,"�e�X�g�I�I");
		
		adv.Set_furiganafont(15,"���C���I");
		adminnum2=texture.LoadImageA("image/sample0003.bmp",&fr);
		chars[0]=texture.LoadImage_Mask("image/orig1_mask.bmp",&fr);
		chars[1]=texture.LoadImageA("image/orig1.bmp",&fr);
		chars[2]=texture.LoadImageA("image/tes.png",&fr);
		//sound.SetSoundVol(soundtest,-1000);
		int test[DateNum];
		for(int i=0;i<5;i++)test[i]=0;
		char teststring[]="test�Ă��ƃe�X�g";
		for(int i=0;teststring[i]!='\0';i++)
		{
			adv.stack_text(teststring[i],test);
		}
		char teststring2[]="����";
		adv.set_furigana_mode();
		for(int i=0;teststring2[i]!='\0';i++)
		{
			adv.stack_text(teststring2[i],test);
		}
		adv.set2_furigana_mode();
		char teststring_f[]="����";
		for(int i=0;teststring_f[i]!='\0';i++)
		{
			adv.stack_text(teststring_f[i],test);
		}
		adv.out_furigana_mode();

		char teststring_3[]="aaaaaaa����������������������������";
		for(int i=0;teststring_3[i]!='\0';i++)
		{
			adv.stack_text(teststring_3[i],test);
		}





		reset_flag=1;
	}


	test_blend+=5*test_blend_f;
	if(test_blend>=255)test_blend_f=-1;
	else if(test_blend<=0)test_blend_f=1;
	//kk=sound.GetCheckPlaying(soundtest);
	texture.Drawing_start();
	texture.DrawImage_Stretch(adminnum2,0,-150,800,800);

	texture.DrawImage(chars[0],400,5);
	texture.DrawImage_Blend(chars[1], chars[0], 400, 5,test_blend);
	//texture.SetBlendMode(MODE_ALPHA, 120);
	texture.DrawImage(chars[2],80,32);
	//text.DrawText(m_pD3DDevice,adminnum,0,0,255,255,255,255,"�e�X�g�\��\n���s�e�X�g");
	//texture.NoBlend(m_pD3DDevice);
	
	adv.Draw_text(adminnum,0,50,800,480,255);
	adv.Draw_furigana(0,50,255);
	btext.DrawBlockText();
	texture.Drawing_end();
	
	return 0;
}
