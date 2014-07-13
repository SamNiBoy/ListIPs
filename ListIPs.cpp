// ListIPs.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ListIPs.h"
#include <winsock2.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;


CString strTemp;
struct hostent *host;

struct in_addr *ptr; // To retrieve the IP Address 

DWORD dwScope = RESOURCE_CONTEXT;
NETRESOURCE *NetResource = NULL;
HANDLE hEnum;
WNetOpenEnum( dwScope, NULL, NULL, 
             NULL, &hEnum );

WSADATA wsaData;
WSAStartup(MAKEWORD(1,1),&wsaData);

if ( hEnum )
{
    DWORD Count = 0xFFFFFFFF;
    DWORD BufferSize = 2048;
    LPVOID Buffer = new char[2048];
    WNetEnumResource( hEnum, &Count, 
        Buffer, &BufferSize );
    NetResource = (NETRESOURCE*)Buffer;

    char szHostName[200];
    unsigned int i;

    for ( i = 0; 
        i < BufferSize/sizeof(NETRESOURCE); 
        i++, NetResource++ )
    {
        if ( NetResource->dwUsage == 
            RESOURCEUSAGE_CONTAINER && 
            NetResource->dwType == 
            RESOURCETYPE_ANY )
        {
            if ( NetResource->lpRemoteName )
            {
                CString strFullName = 
                    NetResource->lpRemoteName;
                if ( 0 == 
                    strFullName.Left(2).Compare("\\\\") )   
                    strFullName = 
                        strFullName.Right(
                            strFullName.GetLength()-2);

                gethostname( szHostName, 
                    strlen( szHostName ) );
                host = gethostbyname(strFullName);

                if(host == NULL) continue; 
                ptr = (struct in_addr *) 
                    host->h_addr_list[0];                    

                // Eg. 211.40.35.76 split up like this.             
                int a = ptr->S_un.S_un_b.s_b1;  // 211           
                int b = ptr->S_un.S_un_b.s_b2;  // 40
                int c = ptr->S_un.S_un_b.s_b3;  // 35
                int d = ptr->S_un.S_un_b.s_b4;  // 76

                strTemp.Format("%s -->  %d.%d.%d.%d",
                    strFullName,a,b,c,d);
                AfxMessageBox(strTemp);
            }
        }
    }

    delete Buffer;
    WNetCloseEnum( hEnum ); 
}

WSACleanup();

	return nRetCode;
}


