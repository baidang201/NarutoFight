#include "AppDelegate.h"
//#include "gamescene/NFGameScene.h"
#include "ui/NFLogoScene.h"

USING_NS_CC;

void copyData(const char* pFileName)
{
	std::string strPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(pFileName);
	unsigned long len=0;
	unsigned char* data =NULL;
	data = CCFileUtils::sharedFileUtils()->getFileData(strPath.c_str(),"r",&len);

	std::string destPath = CCFileUtils::sharedFileUtils()->getWritablePath();
	destPath+= pFileName;

	FILE *pFp=fopen(destPath.c_str(),"w+");
	fwrite(data,sizeof(char),len,pFp);
	fclose(pFp);
	delete []data;
	data=NULL;
}

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
   
	copyData(_TR_STATIC_DATABASE_FILE_NAME_);//若不存在直接复制
	
	// initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

    pDirector->setOpenGLView(pEGLView);
	
	pEGLView->setDesignResolutionSize(720,480,kResolutionFixedHeight);
    // turn on display FPS
    pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    //CCScene *pScene = CNFGameScene::CreateGameScene();
	CCScene *pScene = CNFLogoScene::scene();

    // run
    pDirector->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
