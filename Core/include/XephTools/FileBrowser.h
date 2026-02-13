/*========================================================

 XephTools - FileBrowser
 Copyright (C) 2022 Jon Bogert (jonbogert@gmail.com)

 This software is provided 'as-is', without any express or implied warranty.
 In no event will the authors be held liable for any damages arising from the use of this software.

 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it freely,
 subject to the following restrictions:

 1. The origin of this software must not be misrepresented;
    you must not claim that you wrote the original software.
    If you use this software in a product, an acknowledgment
    in the product documentation would be appreciated but is not required.

 2. Altered source versions must be plainly marked as such,
    and must not be misrepresented as being the original software.

 3. This notice may not be removed or altered from any source distribution.

========================================================*/

#ifndef XE_FILEBROWSER_H
#define XE_FILEBROWSER_H

#include <string>
#include <vector>

#include <Windows.h>
#include <shobjidl.h> 
#include <shlobj.h> 

namespace xe
{
    class FileBrowser
    {
        std::vector<COMDLG_FILTERSPEC> m_filetypes;
        std::wstring m_startpath = L"";
        std::wstring m_saveExt = L"";

    public:
        FileBrowser() = default;

        /// <summary>
        /// Adds file extention and tag to list of available file types
        /// std::wstring ext: the extension (format -> L"*.cpp;*.h;*.hpp")
        /// (opt.) std::wstring desc: the description of the file (L"CPP Files")
        /// </summary>
        void PushFileType(const wchar_t* ext, const wchar_t* desc = L"")
        {
            m_filetypes.push_back({ desc, ext });
        }

        void PopFileType()
        {
            m_filetypes.pop_back();
        }

        void ResetFileTypes()
        {
            m_filetypes.clear();
        }

        void SetStartPath(const std::wstring& startPath)
        {
            m_startpath = startPath;
#ifdef WIN32
            for (wchar_t& c : m_startpath)
            {
                if (c == L'/')
                    c = L'\\';
            }
#endif //WIN32
        }

        void SetDefaultExtension(const std::wstring& ext)
        {
            m_saveExt = ext;
        }

        void ClearStartPath()
        {
            m_startpath = std::wstring();
        }

        std::wstring GetFile() const
        {
            CoInitialize(NULL);
            std::wstring path;
            IFileOpenDialog* dialog;
            HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&dialog));
            if (SUCCEEDED(hr))
            {
                if (!m_filetypes.empty())
                {
                    dialog->SetFileTypes(m_filetypes.size(), m_filetypes.data());
                }
                if (!m_startpath.empty())
                {
                    IShellItem* startFolder;
                    HRESULT hr = SHCreateItemFromParsingName(m_startpath.c_str(), NULL, IID_PPV_ARGS(&startFolder));
                    if (SUCCEEDED(hr))
                    {
                        dialog->SetFolder(startFolder);
                        startFolder->Release();
                    }
                }
                hr = dialog->Show(NULL);
                if (SUCCEEDED(hr))
                {
                    IShellItem* item;
                    hr = dialog->GetResult(&item);
                    if (SUCCEEDED(hr))
                    {
                        PWSTR filePath;
                        hr = item->GetDisplayName(SIGDN_FILESYSPATH, &filePath);
                        path = filePath;
                        CoTaskMemFree(filePath);
                    }
                    item->Release();
                }
                dialog->Release();
            }
            CoUninitialize();
            return path;
        }

        std::wstring SaveFile()
        {
            CoInitialize(NULL);
            IFileSaveDialog* fileSave;
            HRESULT hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL, IID_IFileSaveDialog, reinterpret_cast<void**>(&fileSave));
            std::wstring path;
            if (SUCCEEDED(hr)) {
                fileSave->SetFileTypes(m_filetypes.size(), m_filetypes.data());

                fileSave->SetDefaultExtension(m_saveExt.c_str());

                IShellItem* startFolder;
                hr = SHCreateItemFromParsingName(m_startpath.c_str(), NULL, IID_PPV_ARGS(&startFolder));
                if (SUCCEEDED(hr))
                {
                    fileSave->SetFolder(startFolder);
                    startFolder->Release();
                }

                hr = fileSave->Show(NULL);

                if (SUCCEEDED(hr))
                {
                    IShellItem* item;
                    hr = fileSave->GetResult(&item);
                    if (SUCCEEDED(hr))
                    {
                        PWSTR filePath;
                        hr = item->GetDisplayName(SIGDN_FILESYSPATH, &filePath);
                        path = filePath;
                        CoTaskMemFree(filePath);
                        item->Release();
                    }
                }

                fileSave->Release();
            }

            CoUninitialize();
            return path;
        }

    private:
        static int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
        {
            if (uMsg == BFFM_INITIALIZED)
            {
                LPCTSTR path = reinterpret_cast<LPCTSTR>(lpData);
                ::SendMessage(hwnd, BFFM_SETSELECTION, true, (LPARAM)path);
            }
            return 0;
        }

    public:
        std::wstring LoadFolderCondensed()
        {
            std::wstring path;

            CoInitialize(NULL);
            BROWSEINFO bi = { 0 };

            bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
            bi.lpfn = BrowseCallbackProc;
            bi.lParam = reinterpret_cast<LPARAM>(m_startpath.c_str());

            LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
            if (pidl != 0)
            {
                TCHAR folderPath[MAX_PATH];
                if (SHGetPathFromIDList(pidl, folderPath))
                {
                    path = folderPath;
                }

                IMalloc* imalloc = 0;
                if (SUCCEEDED(SHGetMalloc(&imalloc)))
                {
                    imalloc->Free(pidl);
                    imalloc->Release();
                }
            }
            CoUninitialize();
            return path;
        }

        std::wstring LoadFolder()
        {
            CoInitialize(NULL);
            std::wstring path;

            IFileOpenDialog* dialogue;
            HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&dialogue));

            if (SUCCEEDED(hr)) {
                DWORD options;
                dialogue->GetOptions(&options);
                dialogue->SetOptions(options | FOS_PICKFOLDERS);

                hr = dialogue->Show(NULL);

                if (SUCCEEDED(hr)) {
                    IShellItem* item;
                    hr = dialogue->GetResult(&item);
                    if (SUCCEEDED(hr)) {
                        PWSTR folderPath;
                        hr = item->GetDisplayName(SIGDN_FILESYSPATH, &folderPath);
                        path = folderPath;
                        CoTaskMemFree(folderPath);
                        item->Release();
                    }
                }
                dialogue->Release();
            }
            CoUninitialize();
            return path;
        }
    };
}

#endif //!XE_FILEBROWSER_H
