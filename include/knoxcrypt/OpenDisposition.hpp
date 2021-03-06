/*
  Copyright (c) <2013-2016>, <BenHJ>
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

  1. Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.
  2. Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.
  3. Neither the name of the copyright holder nor the names of its contributors
  may be used to endorse or promote products derived from this software without
  specific prior written permission.

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
*/

#pragma once

#include "knoxcrypt/AppendOrOverwrite.hpp"
#include "knoxcrypt/CreateOrDontCreate.hpp"
#include "knoxcrypt/ReadOrWriteOrBoth.hpp"
#include "knoxcrypt/TruncateOrKeep.hpp"

namespace knoxcrypt
{
    class OpenDisposition
    {
      public:
        OpenDisposition() = delete;
        OpenDisposition(ReadOrWriteOrBoth const &readWrite,
                        AppendOrOverwrite const &append,
                        CreateOrDontCreate const &create,
                        TruncateOrKeep const &trunc)
            : m_readWrite(readWrite)
            , m_append(append)
            , m_create(create)
            , m_trunc(trunc)
        {
        }

        static OpenDisposition buildAppendDisposition()
        {
            return OpenDisposition(ReadOrWriteOrBoth::ReadWrite,
                                   AppendOrOverwrite::Append,
                                   CreateOrDontCreate::Create,
                                   TruncateOrKeep::Keep);
        }

        static OpenDisposition buildReadOnlyDisposition()
        {
            // note it only matters that the first paramter is readonly
            // the others don't matter and can be anything
            return OpenDisposition(ReadOrWriteOrBoth::ReadOnly,
                                   AppendOrOverwrite::Append,
                                   CreateOrDontCreate::DontCreate,
                                   TruncateOrKeep::Keep);
        }

        static OpenDisposition buildWriteOnlyDisposition()
        {
            return OpenDisposition(ReadOrWriteOrBoth::WriteOnly,
                                   AppendOrOverwrite::Append,
                                   CreateOrDontCreate::DontCreate,
                                   TruncateOrKeep::Keep);
        }

        static OpenDisposition buildOverwriteDisposition()
        {
            return OpenDisposition(ReadOrWriteOrBoth::ReadWrite,
                                   AppendOrOverwrite::Overwrite,
                                   CreateOrDontCreate::DontCreate,
                                   TruncateOrKeep::Keep);
        }

        ReadOrWriteOrBoth readWrite() const
        {
            return m_readWrite;
        }

        AppendOrOverwrite append() const
        {
            return m_append;
        }

        CreateOrDontCreate create() const
        {
            return m_create;
        }

        TruncateOrKeep trunc() const
        {
            return m_trunc;
        }

        bool equals(OpenDisposition const &other) const
        {
            return (m_readWrite == other.m_readWrite) &&
                   (m_append    == other.m_append)    &&
                   (m_create    == other.m_create)    &&
                   (m_trunc     == other.m_trunc);
        }

      private:
        ReadOrWriteOrBoth m_readWrite;
        AppendOrOverwrite m_append;
        CreateOrDontCreate m_create;
        TruncateOrKeep m_trunc;

    };
}
