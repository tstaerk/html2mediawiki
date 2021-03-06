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
#include <tidy/tidy.h>
#include <tidy/buffio.h>
#include <stdio.h>
#include <errno.h>

void output(QDomNode node)
{
  QDomNode next;
  if (node.nodeName()=="li") std::cout << "\n* ";
  if (node.nodeName()=="h1") std::cout << "\n= ";
  if (node.nodeName()=="h2") std::cout << "\n== ";
  if (node.nodeName()=="p") std::cout << "\n\n";
  if (node.nodeName()=="br") std::cout << "\n";
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
  // the following code is (c) Charles Reitzel and Dave Raggett, see the package tidy                                                                                                                                                             
  TidyBuffer output = {0};                                                                                               
  TidyBuffer errbuf = {0};                                                                                               
  QString result;                                                                                                        
  int rc = -1;                                                                                                           
  Bool ok;                                                                                                               

  TidyDoc tdoc = tidyCreate();                             // Initialize "document"
  ok = tidyOptSetBool( tdoc, TidyXhtmlOut, yes );          // Convert to XHTML
  if ( ok ) rc = tidySetErrorBuffer( tdoc, &errbuf );      // Capture diagnostics
  tidySetCharEncoding( tdoc, "utf8" );
  if ( rc >= 0 ) rc = tidyParseString( tdoc, input.toUtf8().constData() );      // Parse the input    
  if ( rc >= 0 ) rc = tidyCleanAndRepair( tdoc );          // Tidy it up!        
  if ( rc >= 0 ) rc = tidyRunDiagnostics( tdoc );          // Kvetch             
  if ( rc > 1 )                                            // If error, force output.
    rc = ( tidyOptSetBool(tdoc, TidyForceOutput, yes) ? rc : -1 );                   
  if ( rc >= 0 ) rc = tidySaveBuffer( tdoc, &output );     // Pretty Print           
  if ( rc >= 0 )                                                                     
  {                                                                                                
    char* outputstring; // content of the outputfile                                 

    // find out length of outputstring
    int length=0; // length of outputstring
    byte* string=output.bp;                
    while (*string)                        
    {                                      
      string++;                                                
      length++;                                                
    }                                                          

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
  return result;                                       
}

QString cleanwithtextedit(QString text)
{
  QTextEdit* textedit=new QTextEdit();
  textedit->setHtml(text);
  return textedit->toHtml();
}

int main (int argc, char *argv[])
{
  KAboutData aboutData( "html2mediawiki",
			0,
			ki18n("html2mediawiki"),
			"0.1",
			ki18n("Converts html syntax to mediawiki syntax"),
			KAboutData::License_GPL,
			ki18n("(c) 2008-2009 by Thorsten Staerk"),
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
    QFile inputfile(args->url(0).fileName());
    inputfile.open(QIODevice::ReadOnly);
    QString inputfilecontentqstring=QString::fromUtf8(inputfile.read(inputfile.bytesAvailable()));
    QDomDocument mydom=QDomDocument();
    mydom.setContent(tidy(inputfilecontentqstring));
    QDomNode bodynode(mydom.elementsByTagName("body").at(0));
    output(bodynode);
  }
  else 
  {
    std::cout << "html2mediawiki converts an html file to mediawiki syntax" << std::endl;
    std::cout << "html2mediawiki usage: html2mediawiki <htmlfile>" << std::endl;
    std::cout << "htmlfile must be utf-8 encoded." << std::endl;
  }
}

