/*
Copyright (c) <2013-2014>, <BenHJ>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies,
either expressed or implied, of the FreeBSD Project.
*/

#ifndef TeaSafe_FOLDER_MANAGER_HPP__
#define TeaSafe_FOLDER_MANAGER_HPP__

#include "teasafe/FileEntry.hpp"
#include "teasafe/FolderEntry.hpp"

namespace teasafe
{
    class FolderManager
    {
      public:
        FolderManager(std::string const &name);

        void addFile(std::string const& name, std::iostream &fileData)
        {
            m_thisFolderEntry.addFileEntry(name);
        }
        void addFolder(std::string const& name);

        void getFile(std::string &name, std::ostream &output);
        void getFile(uint64_t const fileId, std::ostream &output);

        void removeFile(std::string const &name);
        void removeFolder(std::string const &name);

        std::vector<std::string> listContents();

      private:
        FolderEntry m_thisFolderEntry;

    };
}

#endif // TeaSafe_FOLDER_MANAGER_HPP__