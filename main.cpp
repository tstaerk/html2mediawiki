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
  if (node.isText()) kDebug() << node.nodeValue();
  if (node.isText()) std::cout << node.nodeValue().toStdString();
  if (node.hasChildNodes()) 
  {
    for (int i=0; i<=node.childNodes().count(); ++i) output(node.childNodes().at(i));
  }
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
  if (args->count()) 
  {
    kDebug() << args->url(0).url();
    QFile inputfile(args->url(0).fileName());
    inputfile.open(QIODevice::ReadOnly);
    kDebug() << inputfile.bytesAvailable();
    inputfilecontent = inputfile.read(inputfile.bytesAvailable());
    kDebug() << inputfilecontent;
    QString inputfilecontentqstring(inputfilecontent);
    kDebug() << inputfilecontentqstring;
    QTextEdit* textedit=new QTextEdit();
    textedit->setText(inputfilecontentqstring);
    kDebug() << textedit->toHtml();
    std::cout << textedit->toHtml().toStdString() << std::endl;
  }
  QDomDocument mydom=QDomDocument();
  mydom.setContent(inputfilecontent);
  kDebug() << mydom.elementsByTagName("html").at(0).nodeName();
  QDomNode htmlnode=mydom.elementsByTagName("html").at(0);
  kDebug() << htmlnode.firstChild().nodeName();
  QDomNode bodynode(mydom.elementsByTagName("body").at(0));
  kDebug() << bodynode.firstChild().nodeName();
  QDomNode node=bodynode;
  while (node.hasChildNodes()) 
  {
    node=node.firstChild();
    kDebug() << node.nodeName();
  }
  output(bodynode);
  kDebug() << "out";
}

