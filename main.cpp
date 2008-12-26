#include <iostream>
#include <KApplication>
#include <KAboutData>
#include <KCmdLineArgs>
#include <kdebug.h>
#include <KMessageBox>
#include <KUrl>
#include <QChar>
#include <QFile> 
#include <QDomDocument>
#include <QTextEdit>
#include <tidy.h>
#include <buffio.h>
#include <stdio.h>
#include <errno.h>

void output(QDomNode node)
{
  QDomNode next;
  kDebug() << node.nodeName();
  if (node.nodeName()=="li") std::cout << "\n* ";
  if (node.nodeName()=="h1") std::cout << "\n= ";
  if (node.nodeName()=="h2") std::cout << "\n== ";
  if (node.nodeName()=="p") std::cout << "\n\n";
  if (node.nodeName()=="br") std::cout << "\n";
  if (node.nodeName()=="auml") std::cout << "&auml;";
  if (node.nodeName()=="ouml") std::cout << "&ouml;";
  if (node.nodeName()=="uuml") std::cout << "&uuml;";
  if (node.isText()) kDebug() << node.nodeValue();
  if (node.isText()) std::cout << QString(node.nodeValue().toUtf8()).toStdString();
  if (node.hasChildNodes()) 
  {
    for (int i=0; i<=node.childNodes().count(); ++i) output(node.childNodes().at(i));
  }
  if (node.nodeName()=="p") std::cout << "\n\n";
  if (node.nodeName()=="h1") std::cout << " =";
  if (node.nodeName()=="h2") std::cout << " ==";
}

QString tidy(QString input)
// take html code and return it converted to xhtml code
{                                                        
  kDebug() << "Entering function";                       
  // the following code is (c) Charles Reitzel and Dave Raggett, see the package tidy                                                                                                                                                             
  TidyBuffer output = {0};                                                                                               
  TidyBuffer errbuf = {0};                                                                                               
  QString result;                                                                                                        
  int rc = -1;                                                                                                           
  Bool ok;                                                                                                               

  TidyDoc tdoc = tidyCreate();                             // Initialize "document"
  ok = tidyOptSetBool( tdoc, TidyXhtmlOut, yes );          // Convert to XHTML
  if ( ok ) rc = tidySetErrorBuffer( tdoc, &errbuf );      // Capture diagnostics
  if ( rc >= 0 ) rc = tidyParseString( tdoc, input.toUtf8().constData() );      // Parse the input    
  if ( rc >= 0 ) rc = tidyCleanAndRepair( tdoc );          // Tidy it up!        
  if ( rc >= 0 ) rc = tidyRunDiagnostics( tdoc );          // Kvetch             
  if ( rc > 1 )                                            // If error, force output.
    rc = ( tidyOptSetBool(tdoc, TidyForceOutput, yes) ? rc : -1 );                   
  if ( rc >= 0 ) rc = tidySaveBuffer( tdoc, &output );     // Pretty Print           
  if ( rc >= 0 )                                                                     
  {                                                                                  
    if ( rc > 0 ) kDebug() << "\\nDiagnostics:\\n\\n\%s" << errbuf.bp;               
    char* outputstring; // content of the outputfile                                 

    // find out length of outputstring
    int length=0; // length of outputstring
    byte* string=output.bp;                
    while (*string)                        
    {                                      
      string++;                                                
      length++;                                                
    }                                                          

    kDebug() << "allocating memory " << length;
    outputstring=(char*)malloc(length);        
    snprintf(outputstring,length,"%s",output.bp);
    result=QString::fromUtf8(outputstring,length);
  }                                                    
  else                                                 
    printf( "A severe error (\%d) occurred.\\n", rc ); 
  tidyBufFree( &output );                              
  tidyBufFree( &errbuf );                              
  tidyRelease( tdoc );
  result=result.replace("&Atilde;&para;","&ouml;");
  kDebug() << result;
  return result;                                       
}

QString cleanwithtextedit(QString text)
{
  QTextEdit* textedit=new QTextEdit();
  textedit->setHtml(text);
  return textedit->toHtml();
}

QString clean(QString text)
// this assumes the input is UTF-8 and the endianness is little
{
  char auml[3];
  char uuml[3];
  auml[0]=195;
  auml[1]=164;
  auml[2]=0;
  uuml[0]=195;
  uuml[1]=188;
  uuml[2]=0;
  QString aumlqstr=QString::fromUtf8(auml,2);
  QString uumlqstr=QString::fromUtf8(uuml,2);
  kDebug() << "((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((" << aumlqstr;
  text.replace(aumlqstr,"&auml;");
  text.replace(uumlqstr,"&uuml;");
  return tidy(text);
}

int main (int argc, char *argv[])
{
  KAboutData aboutData( "html2mediawiki",
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
    inputfilecontent = inputfile.read(inputfile.bytesAvailable());
    kDebug() << "inputfilecontent.data()[0]"<<(byte)inputfilecontent.data()[0];
    kDebug() << "inputfilecontent.data()[1]"<<(byte)inputfilecontent.data()[1];
    QString inputfilecontentqstring=QString::fromUtf8(inputfilecontent);
    QDomDocument mydom=QDomDocument();
    mydom.setContent(clean(inputfilecontentqstring));
    kDebug() << mydom.elementsByTagName("html").at(0).nodeName();
    QDomNode htmlnode=mydom.elementsByTagName("html").at(0);
    kDebug() << htmlnode.firstChild().nodeName();
    QDomNode bodynode(mydom.elementsByTagName("body").at(0));
    kDebug() << bodynode.firstChild().nodeName();
    QDomNode node=bodynode;
    output(bodynode);
  }
  else 
  {
    std::cout << "html2mediawiki converts an html file to mediawiki syntax" << std::endl;
    std::cout << "html2mediawiki usage: html2mediawiki <htmlfile>" << std::endl;
  }
}

