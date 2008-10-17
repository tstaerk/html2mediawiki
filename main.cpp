#include <KApplication>
#include <KAboutData>
#include <KCmdLineArgs>
#include <kdebug.h>
#include <KMessageBox>
#include <KUrl>
#include <QFile> 

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
  KCmdLineOptions options; 
  options.add("+[file]", ki18n("Document to open")); 
  KCmdLineArgs::addCmdLineOptions(options); 
  KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

  KApplication app;
  if (args->count()) 
  {
    kDebug() << args->url(0).url();
    QFile inputfile(args->url(0).fileName());
    inputfile.open(QIODevice::ReadOnly);
    kDebug() << inputfile.bytesAvailable();
    QByteArray inputfilecontent = inputfile.read(inputfile.bytesAvailable());
    kDebug() << inputfilecontent;
    QString inputfilecontentqstring(inputfilecontent);
    kDebug() << inputfilecontentqstring;
  }
}
