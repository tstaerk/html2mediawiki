#include <iostream>
#include <KApplication>
#include <KAboutData>
#include <KCmdLineArgs>
#include <kdebug.h>
#include <KMessageBox>
#include <KUrl>
#include <QFile> 
#include <QDomDocument>
#include <QTextEdit>

void output(QDomNode node)
{
  QDomNode next;
  kDebug() << node.nodeName();
  if (node.nodeName()=="li") std::cout << "\n* ";
  if (node.nodeName()=="h1") std::cout << "= ";
  if (node.nodeName()=="p") std::cout << "\n\n";
  if (node.isText()) kDebug() << node.nodeValue();
  if (node.isText()) std::cout << QString(node.nodeValue().toLocal8Bit()).toStdString();
  if (node.hasChildNodes()) 
  {
    for (int i=0; i<=node.childNodes().count(); ++i) output(node.childNodes().at(i));
  }
  if (node.nodeName()=="p") std::cout << "\n\n";
  if (node.nodeName()=="h1") std::cout << " =";
}

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
  QByteArray inputfilecontent;
  KApplication app;
  QTextEdit* textedit=new QTextEdit();
  if (args->count()) 
  {
    kDebug() << args->url(0).url();
    QFile inputfile(args->url(0).fileName());
    inputfile.open(QIODevice::ReadOnly);
    kDebug() << inputfile.bytesAvailable();
    inputfilecontent = inputfile.read(inputfile.bytesAvailable());
    kDebug() << endl << endl << "inputfilecontent is " << endl << inputfilecontent;
    QString inputfilecontentqstring(inputfilecontent);
    kDebug() << inputfilecontentqstring;
    textedit->setHtml(inputfilecontentqstring);
    kDebug() << textedit->toHtml();
  }
  QDomDocument mydom=QDomDocument();
  mydom.setContent(textedit->toHtml());
  kDebug() << mydom.elementsByTagName("html").at(0).nodeName();
  QDomNode htmlnode=mydom.elementsByTagName("html").at(0);
  kDebug() << htmlnode.firstChild().nodeName();
  QDomNode bodynode(mydom.elementsByTagName("body").at(0));
  kDebug() << bodynode.firstChild().nodeName();
  QDomNode node=bodynode;
  output(bodynode);
}

