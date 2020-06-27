#include <iostream>

//#undef _MSC_VER
//#undef WIN32
#ifdef _MSC_VER
#include <Windows.h>
#include <tchar.h>
#endif

#include <assert.h>
#include "ZUtils.h"
#include "ZRenerder.h"
#include "ZWindow.h"
#include "ZLabel.h"
#include "ZPaintManager.h"
#include "ZApplication.h"
#include "ZBuilder.h"


#include "ZuiLib.h"

using namespace ZuiLib;
#ifdef main
#undef main
#endif // main

int main(int argc, char *argv[])
{
#ifdef _MSC_VER
	SetCurrentDirectory(TEXT("D:\\DevelopmentEnvironment\\ppshuai_SDLGUI\\src\\SDLGUI\\out\\build\\x64-Debug\\SDL2ZUI"));
#endif
	ZApplication app;
	{
		{
			//ÉèÖÃÄ¬ÈÏ×ÖÌå
			ZTextInfo info;
			info.name="msyh.ttf";
			info.size=13;
			info.bold=false;
			info.italic=false;
			ZFontMgr::SetDefaultTextInfo(&info);
		}

		ZWindow wnd;
		wnd.Create("ZUI Engine",ZWindow::WINDOW_POS_CENTER,ZWindow::WINDOW_POS_CENTER,800,600);

		ZuiLib::RECT rcCaption={0,40,0,0};
		wnd.GetManager()->SetCaptionRect(rcCaption);
		ZuiLib::RECT rcSizeBox={3,3,3,3};
		wnd.GetManager()->SetSizeBox(rcSizeBox);
		
		ZBuilder builder;
		if(builder.LoadXml("test1.xml"))
		{
			ZControl* ctl=builder.Create(wnd.GetManager());
			wnd.GetManager()->AttachDialog(ctl);
		}

		/*ZVerticalLayout * ver=new ZVerticalLayout();

		ZuiLib::RECT inset={10,10,10,10};
		ver->SetInset(inset);
		ver->SetChildPadding(10);
		ver->SetBkImage("file='btn.png' source='0,0,96,24' corner='4,4,4,4' mask='#8800ffff'");
		wnd.GetManager()->AttachDialog(ver);

		ZLabel * label=new ZLabel();
		label->SetBkColor(0xff0000ff);
		label->SetFixedWidth(300);
		label->SetFixedHeight(30);
		label->SetText("123456abcdefg");
		label->SetTextColor(0xffffffff);
		label->SetTextStyle(DT_RIGHT | DT_VCENTER);
		ver->Add(label);

		ZButton* btn=new ZButton();
		btn->SetFixedWidth(80);
		btn->SetFixedHeight(24);

		btn->SetTextStyle(DT_END_ELLIPSIS | DT_CENTER);
		btn->SetText("Button435435432543253425234");

		btn->SetNormalImage("file='btn.png' source='0,0,96,24' corner='4,4,4,4'");
		btn->SetHotImage("file='btn.png' source='0,24,96,48' corner='4,4,4,4'");
		btn->SetPushedImage("file='btn.png' source='0,48,96,72' corner='4,4,4,4'");
		ver->Add(btn);*/

		app.DoMessageLoop();
	}
	
	return 0;
}

