#include <KApplication>
#include <KAboutData>
#include <KCmdLineArgs>
#include <KMessageBox>
 
int main (int argc, char *argv[])
{
    KAboutData aboutData(
                         "html2mediawiki",
                         0,
                         ki18n("html2mediawiki"),
                         "0.1",
                         ki18n("Converts html syntax to mediawiki syntax"),
                         KAboutData::License_GPL,
                         ki18n("(c) 2008 by Thorsten Staerk"),
                         ki18n("This is html2mediawiki"),
                         "http://www.staerk.de/thorsten",
                         "bugs@staerk.de");
 
    KCmdLineArgs::init( argc, argv, &aboutData );
    KApplication app;
    KGuiItem yesButton( i18n( "Hello" ), QString(),
                        i18n( "This is a tooltip" ),
                        i18n( "This is a WhatsThis help text." ) );
    KMessageBox::questionYesNo( 0, i18n( "Hello World" ),
                                i18n( "Hello" ), yesButton );
}
