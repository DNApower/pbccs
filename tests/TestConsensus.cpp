// Copyright (c) 2014-2015, Pacific Biosciences of California, Inc.
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted (subject to the limitations in the
// disclaimer below) provided that the following conditions are met:
//
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//
//  * Redistributions in binary form must reproduce the above
//    copyright notice, this list of conditions and the following
//    disclaimer in the documentation and/or other materials provided
//    with the distribution.
//
//  * Neither the name of Pacific Biosciences nor the names of its
//    contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
// NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
// GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY PACIFIC
// BIOSCIENCES AND ITS CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL PACIFIC BIOSCIENCES OR ITS
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
// USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
// OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
// SUCH DAMAGE.



#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <pacbio/ccs/Consensus.h>
#include <pacbio/ccs/ReadId.h>
#include <vector>
#include <pbbam/LocalContextFlags.h>
#include <pbbam/Accuracy.h>
using namespace PacBio::CCS;
typedef ReadType<ReadId> Subread;

TEST(ConsensusTest, TestReadFilter)
{
    std::vector<Subread> data;
    std::string fullRead = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
    auto movieName = std::make_shared<std::string>("fakeName");
    LocalContextFlags flags = LocalContextFlags::ADAPTER_BEFORE | LocalContextFlags::ADAPTER_AFTER;
    for(int i=0; i<10; i++) {

    auto read = Subread{ReadId(movieName, 1,
                    Interval(0, fullRead.size())),
                    fullRead, flags, .99};
    data.emplace_back(read);
    }
    // Nothing filtered
    auto result = FilterReads(data, 10);
    EXPECT_EQ(0, result.second);
    
    // All removed
    auto result2 = FilterReads(data, 1000);
    EXPECT_EQ(10, result2.second);
    
    // Just one 
    auto longRead = fullRead + fullRead + fullRead;
    data.emplace_back(Subread{ReadId(movieName, 1,
                    Interval(0, fullRead.size())),
                    longRead, flags, .99});
    auto result3 = FilterReads(data, 10);
    EXPECT_EQ(1, result3.second);

}