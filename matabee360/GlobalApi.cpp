// Global APIs implementatin for Matabee360 program
// 2014-09-05 
// created by Jianghaifeng
#include "stdafx.h"
#include "GlobalApi.h"
#include "ximage.h"
#include <string.h>
#include <WinInet.h>
#include <locale.h>
#include "matabee360.h"
#include <fstream>

using namespace std;
typedef unsigned char uchar;

const TCHAR* g_scheme_type = _T("http");
const TCHAR* g_url_prefix = _T("webdav.buzzsaw.com");

void LogOutput(LPCWSTR info)
{
	OutputDebugString(info);
#if 1
	static int iscreated = 0;

	if (iscreated == 0) { 
		wfstream logfile(_T("C:\\Matabee360.log"), ios::out);//4-Force360:Force360.log  Matabee360:Matabee360.log  
		if(logfile.is_open()) {
			iscreated = 1;
			logfile.close();
		}
	}

	wfstream logfile(_T("C:\\Matabee360.log"), ios::app);//5-Force360:Force360.log  Matabee360:Matabee360.log  
	if (logfile.is_open())
		logfile.write(info, _tcslen(info));
	logfile.close();
#endif
}

static BOOL ConvertStringToURLCoding(char* s, char* d, int len)
{
	d[0] = 0;
    CStringA strTemp;
    int i = 0;
    while(i < len)
    {
        if ((unsigned)s[i] <= (unsigned char)0x7f)
        {
            if ((s[i] >= '0' && s[i] <= '9') || 
                (s[i] >= 'A' && s[i] <= 'Z') ||
                (s[i] >= 'a' && s[i] <= 'z'))
            {
				int dlen = strlen(d);
                d[dlen] = s[i];
				d[dlen+1] = 0;
            }
            else if (s[i] == ' ')    
            {
				int dlen = strlen(d);
                d[dlen] = '+';
				d[dlen+1] = 0;
            }
			else if (s[i] == '/') // shouldn't it be '%2f' ?
            {
				int dlen = strlen(d);
                d[dlen] = '/';
				d[dlen+1] = 0;
            }
			else if (s[i] == 0)
			{
				int dlen = strlen(d);
                d[dlen] = 0;
				return TRUE;
			}
            else
            {
                strTemp.Format("%%%02X", (unsigned char)s[i]);
				strcat(d,strTemp.GetBuffer(strTemp.GetLength()));
            }
            i++;
        }
        else
        {
            strTemp.Format("%%%02X%%%02X%%%02X", (unsigned char)s[i],
                (unsigned char)s[i + 1], (unsigned char)s[i + 2]);
            strcat(d,strTemp.GetBuffer(strTemp.GetLength()));
            i += 3;
        }
    }
    if (i == 0)
    {
        return FALSE;
    }
    return TRUE;
}
//%2fSEMS%2fpano%2f%e4%b8%ad%e6%96%87%e5%90%8d%e6%b5%8b%e8%af%951%0d%0a
//%2FSEMS%2Fpano%2F%E4%B8%AD%E6%96%87%E5%90%8D%E6%B5%8B%E8%AF%951%00
//%2FSEMS%2Fpano%2F%E4%B8%AD%E6%96%87%E5%90%8D%E6%B5%8B%E8%AF%951s
//SEMS/pano/%E4%B8%AD%E6%96%87%E5%90%8D%E6%B5%8B%E8%AF%951

void ANSItoUTF8_URL(char* strAnsi)
{
	UINT nLen = MultiByteToWideChar(CP_THREAD_ACP,NULL,strAnsi,-1,NULL,NULL);
    WCHAR *wszBuffer = new WCHAR[nLen+1];
    nLen = MultiByteToWideChar(CP_THREAD_ACP,NULL,strAnsi,-1,wszBuffer,nLen);
	wszBuffer[nLen] = 0;
    // 获取转为UTF8多字节后需要的缓冲区大小，创建多字节缓冲区
    nLen = WideCharToMultiByte(CP_UTF8,NULL,wszBuffer,-1,NULL,NULL,NULL,NULL);
    char *szBuffer = new char[nLen+1];
    nLen = WideCharToMultiByte(CP_UTF8,NULL,wszBuffer,-1,szBuffer,nLen,NULL,NULL);
    szBuffer[nLen] = 0;

	ConvertStringToURLCoding(szBuffer, strAnsi, nLen);
    
    delete []wszBuffer;
    delete []szBuffer;
}

void ANSItoUTF8(char* strAnsi)
{
    UINT nLen = MultiByteToWideChar(CP_ACP,NULL,strAnsi,-1,NULL,NULL);
    WCHAR *wszBuffer = new WCHAR[nLen+1];
    nLen = MultiByteToWideChar(CP_ACP,NULL,strAnsi,-1,wszBuffer,nLen);
    wszBuffer[nLen] = 0;
    //获取转为UTF8多字节后需要的缓冲区大小，创建多字节缓冲区
    nLen = WideCharToMultiByte(CP_UTF8,NULL,wszBuffer,-1,NULL,NULL,NULL,NULL);
    char *szBuffer = new char[nLen+1];
    nLen = WideCharToMultiByte(CP_UTF8,NULL,wszBuffer,-1,szBuffer,nLen,NULL,NULL);
    szBuffer[nLen] = 0;

    strcpy(strAnsi, szBuffer);
    //内存清理
    delete []wszBuffer;
    delete []szBuffer;
}  

void get_file_name(TCHAR* path, TCHAR *s)
{
	if (path == NULL || s == NULL)
		return;
	_wsplitpath(path,NULL,NULL,s,NULL);
}

void get_full_name(const TCHAR* path, CString& s)
{
	if (path == NULL)
		return;
	TCHAR name[STR_LONG_LEN], ext[STR_LONG_LEN];
	_wsplitpath(path,NULL,NULL,name,ext);
	s.Format(_T("%s%s"), name, ext);
}

// got s with the last CHARactor '\'
void get_root_dir(CString& s)
{
	CString se;
	get_exec_dir(se);
	// trim the last '\'
	se.TrimRight(_T('\\'));
	// reverse fine the last '\'
	int n = se.ReverseFind(_T('\\'));
	// so, you got the parent directory!!!
	s = se.Left(n+1);
}

void get_exec_dir(CString& s)
{
	TCHAR path[STR_LONG_LEN], drive[STR_LONG_LEN], dir[STR_LONG_LEN];
	GetModuleFileName(NULL, path, STR_LONG_LEN);	
	_wsplitpath(path,drive,dir,NULL,NULL);
	s.Format( _T("%s%s"), drive, dir);
}

void get_time_str(CString& s)
{
	//return a string as "YYYYMMDD_hhmmss"
	SYSTEMTIME st;
	GetLocalTime(&st);
	//GetSystemTime(&st);
	s.Format(_T("%04d%02d%02d_%02d%02d%02d"), st.wYear,
		st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);
}

void parse_url(TCHAR* s, TCHAR* schema, TCHAR* url, TCHAR* dir)
{
	if (s == NULL || url == NULL || dir == NULL)
		return;
	TCHAR* p = s, *q;
	TCHAR* dlm = _T("/");

	if (_tcsstr(s, _T("http://")) != NULL) {
		p += 7;
		_tcscpy(schema, _T("http"));
		if ((q=_tcsstr(p, _T("/"))) != NULL) {
			_tcscpy(dir,q);
			_tcsncpy(url,p,q-p);
			url[q-p] = 0;
		}
	}
	// Add 2015-06-17
	else if (_tcsstr(s, _T("https://")) != NULL) {
		p += 8;
		_tcscpy(schema, _T("https"));
		if ((q=_tcsstr(p, _T("/"))) != NULL) {
			_tcscpy(dir,q);
			_tcsncpy(url,p,q-p);
			url[q-p] = 0;
		}
	}
	else 
	{
		_tcscpy(schema, _T("http"));
		if ((q=_tcsstr(p, _T(".com"))) != NULL) {
			_tcscpy(dir,q+4);
			_tcsncpy(url,p,q-p+4);
			url[q-p+4] = 0;
		}
		else
		{
			//_tcscpy(url,_T("webdav.buzzsaw.com"));
			_tcscpy(url, theApp.pCfg->m_site);
			if (p[0] != _T('/'))
			{
				_tcscpy(dir,_T("/"));
				_tcscat(dir,p);
			}
			else
			{
				_tcscpy(dir,p);
			}
		}
	}
}

void put_str2file(TCHAR *str, FILE *f)
{
	fputws(str, f);
	fputws(_T("/n"),f);
}

void AddSupersceneXMLScene(FILE *fp, int idx)
{
	TCHAR *str;
	TCHAR ch_idx[10];

	_swprintf(ch_idx, _T("%02d"), idx+1);

	fputws(_T("<scene name=\"scene_"), fp);
	fputws(ch_idx, fp);
	fputws(_T("\" title=\""), fp);
	fputws(ch_idx, fp);
	fputws(_T("\" onstart=\"activatemap(0);activatespot(spot1,95,0);\" thumburl=\"panos/"), fp);
	fputws(ch_idx, fp);
	fputws(_T("/thumb.jpg\" lat=\"36.23805556\" lng=\"137.97173333\" heading=\"112.5\">\n"), fp);

	str = _T("<view hlookat=\"-177.83\" vlookat=\"0.02\" fovtype=\"VFOV\" fov=\"90\" maxpixelzoom=\"3.0\" fovmin=\"90\" fovmax=\"120\" limitview=\"range\" vlookatmin=\"-90\"  vlookatmax=\"90\"/>\n");
	fputws(str, fp);

	fputws(_T("<preview url=\"panos/"), fp);
	fputws(ch_idx, fp);
	fputws(_T("/preview.jpg\" /> \n"), fp);
	
	str = _T("<image>\n");
	fputws(str, fp);

	fputws(_T("<cube url=\"panos/"), fp);
	fputws(ch_idx, fp);
	fputws(_T("/pano_%s.jpg\" /> \n"), fp);

	str = _T("</image>\n");
	fputws(str, fp);

	str = _T("</scene>\n");
	fputws(str, fp);

	fputws(_T("\n\n"), fp);
}

int write_index_html(CString in_filename, CString project_name, CString out_filename)
{
	FILE *in_file;
	FILE *out_file;

	TCHAR str[250];

	int i;

	if((in_file = _wfopen(in_filename, _T("r,ccs=UNICODE"))) == NULL || (out_file = _wfopen(out_filename, _T("w,ccs=UNICODE"))) == NULL)
	{
		return 0;
	}

	_tsetlocale(LC_ALL, _T(""));
	for(i = 0; i < 25; i++)
	{
		if(i == 3)
		{
			fgetws(str, 250, in_file);
			fputws(_T("<title>"), out_file);
			fputws(project_name, out_file);
			fputws(_T("</title>"), out_file);
		}
		else
		{
			fgetws(str, 250, in_file);
			fputws(str, out_file);
		}
	}

	fclose(in_file);
	fclose(out_file);
	return 1;
}

int write_superscene_xml(CString in_filename, CString out_filename, int scene_num)
{
	FILE *in_file;
	FILE *out_file;

	TCHAR str[250];

	int i;

	if((in_file = _wfopen(in_filename, _T("r,ccs=UNICODE"))) == NULL || (out_file = _wfopen(out_filename, _T("w,ccs=UNICODE"))) == NULL)
	{
		return 0;
	}

	for(i = 0; i < 42; i++)
	{
		fgetws(str, 250, in_file);
		fputws(str, out_file);
	}

	fputws(_T("\n\n"), out_file);

	for(i = 0; i < scene_num; i++)
	{
		AddSupersceneXMLScene(out_file, i);
	}

	fputws(_T("</krpano>"), out_file);

	fclose(in_file);
	fclose(out_file);

	return 1;
}

void get_container_width(int scene_num, CString& strWidth)
{
	int width;

	if(scene_num >= 15)
	{
		strWidth = _T("100%");
	}
	else
	{
		width = 8 + 102 * scene_num + 10 * (scene_num - 1) + 8;
		strWidth.Format(_T("%d"), width);
	}
}

int write_superscene_skin_xml(CString in_filename, CString out_filename, int scene_num)
{
	FILE *in_file;
	FILE *out_file;

	TCHAR str[300];
	CString strWidth;

	int i;
	int width;

	if((in_file = _wfopen(in_filename, _T("r,ccs=UNICODE"))) == NULL || (out_file = _wfopen(out_filename, _T("w,ccs=UNICODE"))) == NULL)
	{
		return 0;
	}

	if(scene_num >= 15)
	{
		strWidth = _T("100%");
	}
	else
	{
		width = 8 + 102 * scene_num + 10 * (scene_num - 1) + 8;
		strWidth.Format(_T("%d"), width);
	}


	for(i = 0; i < 246; i++)
	{
		if(i == 26)
		{
			fgetws(str, 300, in_file);
			//get_container_width(scene_num, strWidth);
			fputws(_T("<layer name=\"skin_scroll_container\" type=\"container\" bgcolor=\"0x000000\" bgalpha=\"0.5\" align=\"centertop\" width=\""), out_file);
			fputws(strWidth, out_file);
			fputws(_T("\" height=\"100%\" x=\"0\" y=\"0\" maskchildren=\"true\">"), out_file);
		}
		else
		{
			fgetws(str, 300, in_file);
			fputws(str, out_file);
		}
	}
	fclose(in_file);
	fclose(out_file);
}

void create_web_files(CString src, CString dst, CString name, int num)
{
	TCHAR web[5][STR_SHORT_LEN] = {
		_T("index.html"),
		_T("superscene.js"),
		_T("superscene.swf"),
		_T("superscene.xml"),
		_T("superscene_editor.html")
	};

	TCHAR skin[8][STR_SHORT_LEN] = {
		_T("addmark.png"),
		_T("close.png"),
		_T("mark5.png"),
		_T("logo.png"),
		_T("qtvr-cursors.png"),
		_T("superscene.png"),
		_T("superscene_skin.xml"),
		_T("thumbborder.png")
	};

	TCHAR plugins[7][STR_SHORT_LEN] = {
		_T("addMark.swf"),
		_T("dblclick.swf"),
		_T("radar.swf"),
		_T("scrollarea.swf"),
		_T("soundinterface.swf"),
		_T("textfield.swf"),
		_T("vtoureditor.swf")
	};

	int i;
	CString s, d, dir;

	for (i=0; i<5; i++) {
		s.Format( _T("%s\\%s"), src, web[i]);
		d.Format( _T("%s\\%s"), dst, web[i]);
		CopyFile(s, d, FALSE);
	}
	s.Format( _T("%s\\%s"), src, _T("index.html"));
	d.Format( _T("%s\\%s"), dst, _T("index.html"));
	write_index_html(s, name, d);

	s.Format( _T("%s\\%s"), src, _T("superscene.xml"));
	d.Format( _T("%s\\%s"), dst, _T("superscene.xml"));
	write_superscene_xml(s, d, num);

	dir.Format( _T("%s\\%s"), dst, _T("skin"));
	CreateDirectory(dir, NULL);
	for (i=0; i<8; i++) {
		s.Format( _T("%s\\skin\\%s"), src, skin[i]);
		d.Format( _T("%s\\%s"), dir, skin[i]);
		CopyFile(s, d, FALSE);
	}
	s.Format( _T("%s\\skin\\%s"), src, _T("superscene_skin.xml"));
	d.Format( _T("%s\\%s"), dir, _T("superscene_skin.xml"));
	CopyFile(s, d, num);

	dir.Format( _T("%s\\%s"), dst, _T("plugins"));
	CreateDirectory(dir, NULL);
	for (i=0; i<7; i++) {
		s.Format( _T("%s\\plugins\\%s"), src, plugins[i]);
		d.Format( _T("%s\\%s"), dir, plugins[i]);
		CopyFile(s, d, FALSE);
	}

	dir.Format( _T("%s\\%s"), dst, _T("panos"));
	CreateDirectory(dir, NULL);
}

typedef struct{
    unsigned int unWidth;
    unsigned int unHeight;
    unsigned int unBytesPerPixel;
    unsigned char * ucData;
} Image_desc;

typedef struct{
    Image_desc b;
    Image_desc f;
    Image_desc l;
    Image_desc r;
    Image_desc d;
    Image_desc u;
    Image_desc preview;
    Image_desc thumb;
} Image_slice;


void CreateCubeImg(uchar *pano_img, int nPanoRows, uchar *view_img ,int slice_lens, char flag)
{
	int nPanoCols;

	double radius;

	double f;
	int nViewRows,nViewCols;

	double a1,a2,a3,b1,b2,b3,c1,c2,c3;

	int i, j;

	double sta, phi;

	const double Pi = 3.1415926;

	switch(flag)
	{
	case 'l': { sta = +Pi/2; phi = 0; } break;
	case 'f': { sta = 0; phi = 0; } break;
	case 'r': { sta = -Pi/2; phi = 0; } break;
	case 'b': { sta = -Pi; phi = 0; } break;
	case 'u': { sta = 0; phi = +Pi/2; } break;
	case 'd': { sta = 0; phi = -Pi/2; } break;
	default : break;
	}

	nPanoCols = nPanoRows * 2;
	radius = nPanoRows / Pi;

	nViewRows = slice_lens;
	nViewCols = slice_lens;

	f = 0.5 * slice_lens;


	a1 = cos(sta);
	a2 =  - sin(sta) * sin(phi);
	a3 = -sin(sta) * cos(phi);
	b1 = 0;
	b2 = cos(phi);
	b3 = -sin(phi);
	c1 = sin(sta);
	c2 = cos(sta) * sin(phi);
	c3 = cos(sta) * cos(phi);


	for(i = 0; i < nViewRows; ++i)  
	{    
		for (j = 0; j < nViewCols; ++j)  
		{
			double u, v, w;
			double U, V, W;
			double x, y;
			double val = 0;

			int xl, xr, yt, yb;

			uchar v11_c1, v11_c2, v11_c3;
			uchar v12_c1, v12_c2, v12_c3;
			uchar v21_c1, v21_c2, v21_c3;
			uchar v22_c1, v22_c2, v22_c3;


			u = j - nViewCols / 2.0;
			v = i - nViewRows / 2.0;
			w = f;

			U = a1 * u + a2 * v + a3 * w;
			V = b1 * u + b2 * v + b3 * w;
			W = c1 * u + c2 * v + c3 * w;

			if(W < 0) val = Pi;

			x = radius * (atan(U / W) + val) + nPanoCols / 2.0 + 0.5;
			y = radius * atan(V / sqrt(U * U + W * W)) + nPanoRows / 2.0 + 0.5;

			if(x < 0)  x += nPanoCols;
			if(x > nPanoCols -1)  x -= nPanoCols;

			if(y < 0)  y += nPanoRows;
			if(y > nPanoRows -1)  y -= nPanoRows;

			xl = (int)(x);
			xr = (int)(x) + 1;
			yt = (int)(y);
			yb = (int)(y) + 1;

			v11_c1 = pano_img[yt * nPanoCols * 3 + xl * 3 + 0];
			v11_c2 = pano_img[yt * nPanoCols * 3 + xl * 3 + 1];
			v11_c3 = pano_img[yt * nPanoCols * 3 + xl * 3 + 2];

			v12_c1 = pano_img[yt * nPanoCols * 3 + xr * 3 + 0];
			v12_c2 = pano_img[yt * nPanoCols * 3 + xr * 3 + 1];
			v12_c3 = pano_img[yt * nPanoCols * 3 + xr * 3 + 2];

			v21_c1 = pano_img[yb * nPanoCols * 3 + xl * 3 + 0];
			v21_c2 = pano_img[yb * nPanoCols * 3 + xl * 3 + 1];
			v21_c3 = pano_img[yb * nPanoCols * 3 + xl * 3 + 2];

			v22_c1 = pano_img[yb * nPanoCols * 3 + xr * 3 + 0];
			v22_c2 = pano_img[yb * nPanoCols * 3 + xr * 3 + 1];
			v22_c3 = pano_img[yb * nPanoCols * 3 + xr * 3 + 2];

			if(xr > nPanoCols - 1 || yb > nPanoRows - 1)
			{
				view_img[i * nViewCols * 3 + j * 3 + 0] = pano_img[yt * nPanoCols * 3 + xl * 3 + 0];
				view_img[i * nViewCols * 3 + j * 3 + 1] = pano_img[yt * nPanoCols * 3 + xl * 3 + 1];
				view_img[i * nViewCols * 3 + j * 3 + 2] = pano_img[yt * nPanoCols * 3 + xl * 3 + 2];
			}
			else
			{
				double dx = x - xl;
				double dy = y - yt;

				view_img[i * nViewCols * 3 + j * 3 + 0] = v11_c1 * (1 - dx) * (1 - dy) + v12_c1 * dx * (1 - dy) + v21_c1 * (1 - dx) * dy + v22_c1 * dx * dy;
				view_img[i * nViewCols * 3 + j * 3 + 1] = v11_c2 * (1 - dx) * (1 - dy) + v12_c2 * dx * (1 - dy) + v21_c2 * (1 - dx) * dy + v22_c2 * dx * dy;
				view_img[i * nViewCols * 3 + j * 3 + 2] = v11_c3 * (1 - dx) * (1 - dy) + v12_c3 * dx * (1 - dy) + v21_c3 * (1 - dx) * dy + v22_c3 * dx * dy;
			}
		}  
	}
}

void GetThumbImg(Image_desc *src, Image_desc *dst)
{
	double scale;
	int i, j;
	int m, n;
	int src_len, dst_len;
    
	src_len = src->unWidth;
	dst_len = dst->unWidth;
    
	for(i = 0; i < dst_len; i++)
	{
		for(j = 0; j < dst_len; j++)
		{
			scale = (double)src_len / dst_len;
			m = (int)(i * scale + 0.5);
			n = (int)(j * scale + 0.5);
            
			dst->ucData[i * dst_len * 3 + j * 3 + 0] = src->ucData[m * src_len * 3 + n * 3 + 0];
			dst->ucData[i * dst_len * 3 + j * 3 + 1] = src->ucData[m * src_len * 3 + n * 3 + 1];
			dst->ucData[i * dst_len * 3 + j * 3 + 2] = src->ucData[m * src_len * 3 + n * 3 + 2];
		}
	}
}

int cube_slice(const Image_desc *pano, Image_slice *slice)
{
	int i, j;
	int m, n;
	int pre_len;
	int cub_len;
	double scale;
	int nPanoRows,nPanoCols, nPixelNum;
	unsigned char *pano_img;
    
	nPanoRows = pano->unHeight;
	nPanoCols = pano->unWidth;
	nPixelNum = pano->unBytesPerPixel;
	pano_img = pano->ucData;
    
	CreateCubeImg(pano_img, nPanoRows, slice->l.ucData, slice->l.unWidth, 'l');
	CreateCubeImg(pano_img, nPanoRows, slice->r.ucData, slice->r.unWidth, 'r');
	CreateCubeImg(pano_img, nPanoRows, slice->f.ucData, slice->f.unWidth, 'f');
	CreateCubeImg(pano_img, nPanoRows, slice->b.ucData, slice->b.unWidth, 'b');
	CreateCubeImg(pano_img, nPanoRows, slice->u.ucData, slice->u.unWidth, 'u');
	CreateCubeImg(pano_img, nPanoRows, slice->d.ucData, slice->d.unWidth, 'd');
    
	GetThumbImg(&(slice->f), &(slice->thumb));
    
	pre_len = slice->preview.unWidth;
	cub_len = slice->f.unWidth;
    
	scale = (double)slice->f.unWidth/slice->preview.unWidth;
    
	for(i = 0; i < pre_len; i++)
	{
		for(j = 0; j < pre_len; j++)
		{
			m = (int)(i * scale);
			n = (int)(j * scale);
            
			slice->preview.ucData[(i + pre_len * 0) * pre_len * 3 + j * 3 + 0] = slice->u.ucData[m * cub_len * 3 + n * 3 + 0];
			slice->preview.ucData[(i + pre_len * 0) * pre_len * 3 + j * 3 + 1] = slice->u.ucData[m * cub_len * 3 + n * 3 + 1];
			slice->preview.ucData[(i + pre_len * 0) * pre_len * 3 + j * 3 + 2] = slice->u.ucData[m * cub_len * 3 + n * 3 + 2];
            
			slice->preview.ucData[(i + pre_len * 1) * pre_len * 3 + j * 3 + 0] = slice->d.ucData[m * cub_len * 3 + n * 3 + 0];
			slice->preview.ucData[(i + pre_len * 1) * pre_len * 3 + j * 3 + 1] = slice->d.ucData[m * cub_len * 3 + n * 3 + 1];
			slice->preview.ucData[(i + pre_len * 1) * pre_len * 3 + j * 3 + 2] = slice->d.ucData[m * cub_len * 3 + n * 3 + 2];
            
			slice->preview.ucData[(i + pre_len * 2) * pre_len * 3 + j * 3 + 0] = slice->b.ucData[m * cub_len * 3 + n * 3 + 0];
			slice->preview.ucData[(i + pre_len * 2) * pre_len * 3 + j * 3 + 1] = slice->b.ucData[m * cub_len * 3 + n * 3 + 1];
			slice->preview.ucData[(i + pre_len * 2) * pre_len * 3 + j * 3 + 2] = slice->b.ucData[m * cub_len * 3 + n * 3 + 2];
            
			slice->preview.ucData[(i + pre_len * 3) * pre_len * 3 + j * 3 + 0] = slice->r.ucData[m * cub_len * 3 + n * 3 + 0];
			slice->preview.ucData[(i + pre_len * 3) * pre_len * 3 + j * 3 + 1] = slice->r.ucData[m * cub_len * 3 + n * 3 + 1];
			slice->preview.ucData[(i + pre_len * 3) * pre_len * 3 + j * 3 + 2] = slice->r.ucData[m * cub_len * 3 + n * 3 + 2];
            
			slice->preview.ucData[(i + pre_len * 4) * pre_len * 3 + j * 3 + 0] = slice->f.ucData[m * cub_len * 3 + n * 3 + 0];
			slice->preview.ucData[(i + pre_len * 4) * pre_len * 3 + j * 3 + 1] = slice->f.ucData[m * cub_len * 3 + n * 3 + 1];
			slice->preview.ucData[(i + pre_len * 4) * pre_len * 3 + j * 3 + 2] = slice->f.ucData[m * cub_len * 3 + n * 3 + 2];
            
			slice->preview.ucData[(i + pre_len * 5) * pre_len * 3 + j * 3 + 0] = slice->l.ucData[m * cub_len * 3 + n * 3 + 0];
			slice->preview.ucData[(i + pre_len * 5) * pre_len * 3 + j * 3 + 1] = slice->l.ucData[m * cub_len * 3 + n * 3 + 1];
			slice->preview.ucData[(i + pre_len * 5) * pre_len * 3 + j * 3 + 2] = slice->l.ucData[m * cub_len * 3 + n * 3 + 2];
		}
	}
    
	return(1);
}

bool compress_save_cube(Image_desc * slice,TCHAR* dest_path, TCHAR* name, int quality)
{
	TCHAR filename[STR_LONG_LEN];
	swprintf(filename, _T("%s\\%s.jpg"), dest_path, name);

	CxImage slice_img;
	slice_img.SetJpegQuality(quality);
	slice_img.CreateFromArray(slice->ucData, slice->unWidth, slice->unHeight, 24, slice->unWidth*3, false);
	slice_img.Save(filename, CXIMAGE_FORMAT_JPG);

	return true;
}

bool slice_routine(int lens, int thumb_lens,int preview_lens, TCHAR* source_file, TCHAR* dest_path,int quality)
{
    Image_desc pano = {0};
	CxImage img(source_file, CXIMAGE_FORMAT_BMP);

	pano.unHeight = img.GetHeight();
	pano.unWidth  = img.GetWidth();
	pano.unBytesPerPixel = img.GetBpp()/8;
	pano.ucData = (unsigned char*)malloc(pano.unWidth*pano.unHeight*pano.unBytesPerPixel);
	memcpy(pano.ucData, img.GetBits(), pano.unWidth*pano.unHeight*pano.unBytesPerPixel);

    /*************************************************************/
    Image_slice *cubes;
	cubes = (Image_slice*)malloc(sizeof(Image_slice));
    
	cubes->f.unWidth = lens;
	cubes->f.unHeight = lens;
	cubes->f.unBytesPerPixel = 3;
	cubes->f.ucData = (unsigned char*)malloc(lens * lens * 3);
    
	cubes->b.unWidth = lens;
	cubes->b.unHeight = lens;
	cubes->b.unBytesPerPixel = 3;
	cubes->b.ucData = (unsigned char*)malloc(lens * lens * 3);
    
	cubes->l.unWidth = lens;
	cubes->l.unHeight = lens;
	cubes->l.unBytesPerPixel = 3;
	cubes->l.ucData = (unsigned char*)malloc(lens * lens * 3);
    
	cubes->r.unWidth = lens;
	cubes->r.unHeight = lens;
	cubes->r.unBytesPerPixel = 3;
	cubes->r.ucData = (unsigned char*)malloc(lens * lens * 3);
    
	cubes->u.unWidth = lens;
	cubes->u.unHeight = lens;
	cubes->u.unBytesPerPixel = 3;
	cubes->u.ucData = (unsigned char*)malloc(lens * lens * 3);
    
	cubes->d.unWidth = lens;
	cubes->d.unHeight = lens;
	cubes->d.unBytesPerPixel = 3;
	cubes->d.ucData = (unsigned char*)malloc(lens * lens * 3);
    
	cubes->thumb.unWidth = thumb_lens;
	cubes->thumb.unHeight = thumb_lens;
	cubes->thumb.unBytesPerPixel = 3;
	cubes->thumb.ucData = (unsigned char*)malloc(thumb_lens * thumb_lens * 3);
    
	cubes->preview.unWidth = preview_lens;
	cubes->preview.unHeight = preview_lens * 6;
	cubes->preview.unBytesPerPixel = 3;
	cubes->preview.ucData = (unsigned char*)malloc(preview_lens * 6 * preview_lens * 3);
    
	cube_slice(&pano, cubes);
    /*************************************************************/
    compress_save_cube(&(cubes->f),			dest_path, _T("pano_f"),  quality);
    compress_save_cube(&(cubes->b),			dest_path, _T("pano_b"),  quality);
    compress_save_cube(&(cubes->l),			dest_path, _T("pano_l"),  quality);
    compress_save_cube(&(cubes->r),			dest_path, _T("pano_r"),  quality);
    compress_save_cube(&(cubes->u),			dest_path, _T("pano_d"),  quality);
    compress_save_cube(&(cubes->d),			dest_path, _T("pano_u"),  quality);
    compress_save_cube(&(cubes->preview),	dest_path, _T("preview"), quality);
    compress_save_cube(&(cubes->thumb),		dest_path, _T("thumb"),   quality);
    
    free(cubes->f.ucData);
    free(cubes->b.ucData);
    free(cubes->l.ucData);
    free(cubes->r.ucData);
    free(cubes->u.ucData);
    free(cubes->d.ucData);
    free(cubes->preview.ucData);
    free(cubes->thumb.ucData);
    free(cubes);
    free(pano.ucData);

	return true;
}