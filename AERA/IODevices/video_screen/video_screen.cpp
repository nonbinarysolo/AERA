//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//_/_/
//_/_/ AERA
//_/_/ Autocatalytic Endogenous Reflective Architecture
//_/_/ 
//_/_/ Copyright (c) 2022-2023 Jeff Thompson
//_/_/ Copyright (c) 2022-2023 Icelandic Institute for Intelligent Machines
//_/_/ http://www.iiim.is
//_/_/
//_/_/ --- Open-Source BSD License, with CADIA Clause v 1.0 ---
//_/_/
//_/_/ Redistribution and use in source and binary forms, with or without
//_/_/ modification, is permitted provided that the following conditions
//_/_/ are met:
//_/_/ - Redistributions of source code must retain the above copyright
//_/_/   and collaboration notice, this list of conditions and the
//_/_/   following disclaimer.
//_/_/ - Redistributions in binary form must reproduce the above copyright
//_/_/   notice, this list of conditions and the following disclaimer 
//_/_/   in the documentation and/or other materials provided with 
//_/_/   the distribution.
//_/_/
//_/_/ - Neither the name of its copyright holders nor the names of its
//_/_/   contributors may be used to endorse or promote products
//_/_/   derived from this software without specific prior 
//_/_/   written permission.
//_/_/   
//_/_/ - CADIA Clause: The license granted in and to the software 
//_/_/   under this agreement is a limited-use license. 
//_/_/   The software may not be used in furtherance of:
//_/_/    (i)   intentionally causing bodily injury or severe emotional 
//_/_/          distress to any person;
//_/_/    (ii)  invading the personal privacy or violating the human 
//_/_/          rights of any person; or
//_/_/    (iii) committing or preparing for any act of war.
//_/_/
//_/_/ THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
//_/_/ CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
//_/_/ INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
//_/_/ MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
//_/_/ DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR 
//_/_/ CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
//_/_/ SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
//_/_/ BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
//_/_/ SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
//_/_/ INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
//_/_/ WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
//_/_/ NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
//_/_/ OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
//_/_/ OF SUCH DAMAGE.
//_/_/ 
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

#include "../r_exec/mem.h"
#include "video_screen.h"

using namespace std;
using namespace r_code;
using namespace r_exec;

namespace video_screen {

#define N_FOVEA_PATTERNS 512

  VideoScreen::VideoScreen(int width, int height)
: width_(width),
  height_(height),
  screen_(new bool[width * height]),
  fovea_x_(3),
  fovea_y_(5),
  fovea_patterns_(NULL) {
  clear_screen();
}

bool VideoScreen::load(vector<Code*> *objects) {
  // Draw an initial objects.
  draw_paddle(3, 5);
  draw_ball(10, 10);

  if (fovea_patterns_)
    // We don't expect this. load should only be called once.
    delete fovea_patterns_;

  // See the order in fovea-patterns.replicode .
  fovea_patterns_ = new Code*[N_FOVEA_PATTERNS] {
    _Mem::find_object(objects, "pattern0"),
    _Mem::find_object(objects, "pattern1"),
    _Mem::find_object(objects, "pattern2"),
    _Mem::find_object(objects, "pattern12"),
    _Mem::find_object(objects, "pattern3"),
    _Mem::find_object(objects, "pattern13"),
    _Mem::find_object(objects, "pattern23"),
    _Mem::find_object(objects, "pattern123"),
    _Mem::find_object(objects, "pattern4"),
    _Mem::find_object(objects, "pattern14"),
    _Mem::find_object(objects, "pattern24"),
    _Mem::find_object(objects, "pattern124"),
    _Mem::find_object(objects, "pattern34"),
    _Mem::find_object(objects, "pattern134"),
    _Mem::find_object(objects, "pattern234"),
    _Mem::find_object(objects, "pattern1234"),
    _Mem::find_object(objects, "pattern5"),
    _Mem::find_object(objects, "pattern15"),
    _Mem::find_object(objects, "pattern25"),
    _Mem::find_object(objects, "pattern125"),
    _Mem::find_object(objects, "pattern35"),
    _Mem::find_object(objects, "pattern135"),
    _Mem::find_object(objects, "pattern235"),
    _Mem::find_object(objects, "pattern1235"),
    _Mem::find_object(objects, "pattern45"),
    _Mem::find_object(objects, "pattern145"),
    _Mem::find_object(objects, "pattern245"),
    _Mem::find_object(objects, "pattern1245"),
    _Mem::find_object(objects, "pattern345"),
    _Mem::find_object(objects, "pattern1345"),
    _Mem::find_object(objects, "pattern2345"),
    _Mem::find_object(objects, "pattern12345"),
    _Mem::find_object(objects, "pattern6"),
    _Mem::find_object(objects, "pattern16"),
    _Mem::find_object(objects, "pattern26"),
    _Mem::find_object(objects, "pattern126"),
    _Mem::find_object(objects, "pattern36"),
    _Mem::find_object(objects, "pattern136"),
    _Mem::find_object(objects, "pattern236"),
    _Mem::find_object(objects, "pattern1236"),
    _Mem::find_object(objects, "pattern46"),
    _Mem::find_object(objects, "pattern146"),
    _Mem::find_object(objects, "pattern246"),
    _Mem::find_object(objects, "pattern1246"),
    _Mem::find_object(objects, "pattern346"),
    _Mem::find_object(objects, "pattern1346"),
    _Mem::find_object(objects, "pattern2346"),
    _Mem::find_object(objects, "pattern12346"),
    _Mem::find_object(objects, "pattern56"),
    _Mem::find_object(objects, "pattern156"),
    _Mem::find_object(objects, "pattern256"),
    _Mem::find_object(objects, "pattern1256"),
    _Mem::find_object(objects, "pattern356"),
    _Mem::find_object(objects, "pattern1356"),
    _Mem::find_object(objects, "pattern2356"),
    _Mem::find_object(objects, "pattern12356"),
    _Mem::find_object(objects, "pattern456"),
    _Mem::find_object(objects, "pattern1456"),
    _Mem::find_object(objects, "pattern2456"),
    _Mem::find_object(objects, "pattern12456"),
    _Mem::find_object(objects, "pattern3456"),
    _Mem::find_object(objects, "pattern13456"),
    _Mem::find_object(objects, "pattern23456"),
    _Mem::find_object(objects, "pattern123456"),
    _Mem::find_object(objects, "pattern7"),
    _Mem::find_object(objects, "pattern17"),
    _Mem::find_object(objects, "pattern27"),
    _Mem::find_object(objects, "pattern127"),
    _Mem::find_object(objects, "pattern37"),
    _Mem::find_object(objects, "pattern137"),
    _Mem::find_object(objects, "pattern237"),
    _Mem::find_object(objects, "pattern1237"),
    _Mem::find_object(objects, "pattern47"),
    _Mem::find_object(objects, "pattern147"),
    _Mem::find_object(objects, "pattern247"),
    _Mem::find_object(objects, "pattern1247"),
    _Mem::find_object(objects, "pattern347"),
    _Mem::find_object(objects, "pattern1347"),
    _Mem::find_object(objects, "pattern2347"),
    _Mem::find_object(objects, "pattern12347"),
    _Mem::find_object(objects, "pattern57"),
    _Mem::find_object(objects, "pattern157"),
    _Mem::find_object(objects, "pattern257"),
    _Mem::find_object(objects, "pattern1257"),
    _Mem::find_object(objects, "pattern357"),
    _Mem::find_object(objects, "pattern1357"),
    _Mem::find_object(objects, "pattern2357"),
    _Mem::find_object(objects, "pattern12357"),
    _Mem::find_object(objects, "pattern457"),
    _Mem::find_object(objects, "pattern1457"),
    _Mem::find_object(objects, "pattern2457"),
    _Mem::find_object(objects, "pattern12457"),
    _Mem::find_object(objects, "pattern3457"),
    _Mem::find_object(objects, "pattern13457"),
    _Mem::find_object(objects, "pattern23457"),
    _Mem::find_object(objects, "pattern123457"),
    _Mem::find_object(objects, "pattern67"),
    _Mem::find_object(objects, "pattern167"),
    _Mem::find_object(objects, "pattern267"),
    _Mem::find_object(objects, "pattern1267"),
    _Mem::find_object(objects, "pattern367"),
    _Mem::find_object(objects, "pattern1367"),
    _Mem::find_object(objects, "pattern2367"),
    _Mem::find_object(objects, "pattern12367"),
    _Mem::find_object(objects, "pattern467"),
    _Mem::find_object(objects, "pattern1467"),
    _Mem::find_object(objects, "pattern2467"),
    _Mem::find_object(objects, "pattern12467"),
    _Mem::find_object(objects, "pattern3467"),
    _Mem::find_object(objects, "pattern13467"),
    _Mem::find_object(objects, "pattern23467"),
    _Mem::find_object(objects, "pattern123467"),
    _Mem::find_object(objects, "pattern567"),
    _Mem::find_object(objects, "pattern1567"),
    _Mem::find_object(objects, "pattern2567"),
    _Mem::find_object(objects, "pattern12567"),
    _Mem::find_object(objects, "pattern3567"),
    _Mem::find_object(objects, "pattern13567"),
    _Mem::find_object(objects, "pattern23567"),
    _Mem::find_object(objects, "pattern123567"),
    _Mem::find_object(objects, "pattern4567"),
    _Mem::find_object(objects, "pattern14567"),
    _Mem::find_object(objects, "pattern24567"),
    _Mem::find_object(objects, "pattern124567"),
    _Mem::find_object(objects, "pattern34567"),
    _Mem::find_object(objects, "pattern134567"),
    _Mem::find_object(objects, "pattern234567"),
    _Mem::find_object(objects, "pattern1234567"),
    _Mem::find_object(objects, "pattern8"),
    _Mem::find_object(objects, "pattern18"),
    _Mem::find_object(objects, "pattern28"),
    _Mem::find_object(objects, "pattern128"),
    _Mem::find_object(objects, "pattern38"),
    _Mem::find_object(objects, "pattern138"),
    _Mem::find_object(objects, "pattern238"),
    _Mem::find_object(objects, "pattern1238"),
    _Mem::find_object(objects, "pattern48"),
    _Mem::find_object(objects, "pattern148"),
    _Mem::find_object(objects, "pattern248"),
    _Mem::find_object(objects, "pattern1248"),
    _Mem::find_object(objects, "pattern348"),
    _Mem::find_object(objects, "pattern1348"),
    _Mem::find_object(objects, "pattern2348"),
    _Mem::find_object(objects, "pattern12348"),
    _Mem::find_object(objects, "pattern58"),
    _Mem::find_object(objects, "pattern158"),
    _Mem::find_object(objects, "pattern258"),
    _Mem::find_object(objects, "pattern1258"),
    _Mem::find_object(objects, "pattern358"),
    _Mem::find_object(objects, "pattern1358"),
    _Mem::find_object(objects, "pattern2358"),
    _Mem::find_object(objects, "pattern12358"),
    _Mem::find_object(objects, "pattern458"),
    _Mem::find_object(objects, "pattern1458"),
    _Mem::find_object(objects, "pattern2458"),
    _Mem::find_object(objects, "pattern12458"),
    _Mem::find_object(objects, "pattern3458"),
    _Mem::find_object(objects, "pattern13458"),
    _Mem::find_object(objects, "pattern23458"),
    _Mem::find_object(objects, "pattern123458"),
    _Mem::find_object(objects, "pattern68"),
    _Mem::find_object(objects, "pattern168"),
    _Mem::find_object(objects, "pattern268"),
    _Mem::find_object(objects, "pattern1268"),
    _Mem::find_object(objects, "pattern368"),
    _Mem::find_object(objects, "pattern1368"),
    _Mem::find_object(objects, "pattern2368"),
    _Mem::find_object(objects, "pattern12368"),
    _Mem::find_object(objects, "pattern468"),
    _Mem::find_object(objects, "pattern1468"),
    _Mem::find_object(objects, "pattern2468"),
    _Mem::find_object(objects, "pattern12468"),
    _Mem::find_object(objects, "pattern3468"),
    _Mem::find_object(objects, "pattern13468"),
    _Mem::find_object(objects, "pattern23468"),
    _Mem::find_object(objects, "pattern123468"),
    _Mem::find_object(objects, "pattern568"),
    _Mem::find_object(objects, "pattern1568"),
    _Mem::find_object(objects, "pattern2568"),
    _Mem::find_object(objects, "pattern12568"),
    _Mem::find_object(objects, "pattern3568"),
    _Mem::find_object(objects, "pattern13568"),
    _Mem::find_object(objects, "pattern23568"),
    _Mem::find_object(objects, "pattern123568"),
    _Mem::find_object(objects, "pattern4568"),
    _Mem::find_object(objects, "pattern14568"),
    _Mem::find_object(objects, "pattern24568"),
    _Mem::find_object(objects, "pattern124568"),
    _Mem::find_object(objects, "pattern34568"),
    _Mem::find_object(objects, "pattern134568"),
    _Mem::find_object(objects, "pattern234568"),
    _Mem::find_object(objects, "pattern1234568"),
    _Mem::find_object(objects, "pattern78"),
    _Mem::find_object(objects, "pattern178"),
    _Mem::find_object(objects, "pattern278"),
    _Mem::find_object(objects, "pattern1278"),
    _Mem::find_object(objects, "pattern378"),
    _Mem::find_object(objects, "pattern1378"),
    _Mem::find_object(objects, "pattern2378"),
    _Mem::find_object(objects, "pattern12378"),
    _Mem::find_object(objects, "pattern478"),
    _Mem::find_object(objects, "pattern1478"),
    _Mem::find_object(objects, "pattern2478"),
    _Mem::find_object(objects, "pattern12478"),
    _Mem::find_object(objects, "pattern3478"),
    _Mem::find_object(objects, "pattern13478"),
    _Mem::find_object(objects, "pattern23478"),
    _Mem::find_object(objects, "pattern123478"),
    _Mem::find_object(objects, "pattern578"),
    _Mem::find_object(objects, "pattern1578"),
    _Mem::find_object(objects, "pattern2578"),
    _Mem::find_object(objects, "pattern12578"),
    _Mem::find_object(objects, "pattern3578"),
    _Mem::find_object(objects, "pattern13578"),
    _Mem::find_object(objects, "pattern23578"),
    _Mem::find_object(objects, "pattern123578"),
    _Mem::find_object(objects, "pattern4578"),
    _Mem::find_object(objects, "pattern14578"),
    _Mem::find_object(objects, "pattern24578"),
    _Mem::find_object(objects, "pattern124578"),
    _Mem::find_object(objects, "pattern34578"),
    _Mem::find_object(objects, "pattern134578"),
    _Mem::find_object(objects, "pattern234578"),
    _Mem::find_object(objects, "pattern1234578"),
    _Mem::find_object(objects, "pattern678"),
    _Mem::find_object(objects, "pattern1678"),
    _Mem::find_object(objects, "pattern2678"),
    _Mem::find_object(objects, "pattern12678"),
    _Mem::find_object(objects, "pattern3678"),
    _Mem::find_object(objects, "pattern13678"),
    _Mem::find_object(objects, "pattern23678"),
    _Mem::find_object(objects, "pattern123678"),
    _Mem::find_object(objects, "pattern4678"),
    _Mem::find_object(objects, "pattern14678"),
    _Mem::find_object(objects, "pattern24678"),
    _Mem::find_object(objects, "pattern124678"),
    _Mem::find_object(objects, "pattern34678"),
    _Mem::find_object(objects, "pattern134678"),
    _Mem::find_object(objects, "pattern234678"),
    _Mem::find_object(objects, "pattern1234678"),
    _Mem::find_object(objects, "pattern5678"),
    _Mem::find_object(objects, "pattern15678"),
    _Mem::find_object(objects, "pattern25678"),
    _Mem::find_object(objects, "pattern125678"),
    _Mem::find_object(objects, "pattern35678"),
    _Mem::find_object(objects, "pattern135678"),
    _Mem::find_object(objects, "pattern235678"),
    _Mem::find_object(objects, "pattern1235678"),
    _Mem::find_object(objects, "pattern45678"),
    _Mem::find_object(objects, "pattern145678"),
    _Mem::find_object(objects, "pattern245678"),
    _Mem::find_object(objects, "pattern1245678"),
    _Mem::find_object(objects, "pattern345678"),
    _Mem::find_object(objects, "pattern1345678"),
    _Mem::find_object(objects, "pattern2345678"),
    _Mem::find_object(objects, "pattern12345678"),
    _Mem::find_object(objects, "pattern9"),
    _Mem::find_object(objects, "pattern19"),
    _Mem::find_object(objects, "pattern29"),
    _Mem::find_object(objects, "pattern129"),
    _Mem::find_object(objects, "pattern39"),
    _Mem::find_object(objects, "pattern139"),
    _Mem::find_object(objects, "pattern239"),
    _Mem::find_object(objects, "pattern1239"),
    _Mem::find_object(objects, "pattern49"),
    _Mem::find_object(objects, "pattern149"),
    _Mem::find_object(objects, "pattern249"),
    _Mem::find_object(objects, "pattern1249"),
    _Mem::find_object(objects, "pattern349"),
    _Mem::find_object(objects, "pattern1349"),
    _Mem::find_object(objects, "pattern2349"),
    _Mem::find_object(objects, "pattern12349"),
    _Mem::find_object(objects, "pattern59"),
    _Mem::find_object(objects, "pattern159"),
    _Mem::find_object(objects, "pattern259"),
    _Mem::find_object(objects, "pattern1259"),
    _Mem::find_object(objects, "pattern359"),
    _Mem::find_object(objects, "pattern1359"),
    _Mem::find_object(objects, "pattern2359"),
    _Mem::find_object(objects, "pattern12359"),
    _Mem::find_object(objects, "pattern459"),
    _Mem::find_object(objects, "pattern1459"),
    _Mem::find_object(objects, "pattern2459"),
    _Mem::find_object(objects, "pattern12459"),
    _Mem::find_object(objects, "pattern3459"),
    _Mem::find_object(objects, "pattern13459"),
    _Mem::find_object(objects, "pattern23459"),
    _Mem::find_object(objects, "pattern123459"),
    _Mem::find_object(objects, "pattern69"),
    _Mem::find_object(objects, "pattern169"),
    _Mem::find_object(objects, "pattern269"),
    _Mem::find_object(objects, "pattern1269"),
    _Mem::find_object(objects, "pattern369"),
    _Mem::find_object(objects, "pattern1369"),
    _Mem::find_object(objects, "pattern2369"),
    _Mem::find_object(objects, "pattern12369"),
    _Mem::find_object(objects, "pattern469"),
    _Mem::find_object(objects, "pattern1469"),
    _Mem::find_object(objects, "pattern2469"),
    _Mem::find_object(objects, "pattern12469"),
    _Mem::find_object(objects, "pattern3469"),
    _Mem::find_object(objects, "pattern13469"),
    _Mem::find_object(objects, "pattern23469"),
    _Mem::find_object(objects, "pattern123469"),
    _Mem::find_object(objects, "pattern569"),
    _Mem::find_object(objects, "pattern1569"),
    _Mem::find_object(objects, "pattern2569"),
    _Mem::find_object(objects, "pattern12569"),
    _Mem::find_object(objects, "pattern3569"),
    _Mem::find_object(objects, "pattern13569"),
    _Mem::find_object(objects, "pattern23569"),
    _Mem::find_object(objects, "pattern123569"),
    _Mem::find_object(objects, "pattern4569"),
    _Mem::find_object(objects, "pattern14569"),
    _Mem::find_object(objects, "pattern24569"),
    _Mem::find_object(objects, "pattern124569"),
    _Mem::find_object(objects, "pattern34569"),
    _Mem::find_object(objects, "pattern134569"),
    _Mem::find_object(objects, "pattern234569"),
    _Mem::find_object(objects, "pattern1234569"),
    _Mem::find_object(objects, "pattern79"),
    _Mem::find_object(objects, "pattern179"),
    _Mem::find_object(objects, "pattern279"),
    _Mem::find_object(objects, "pattern1279"),
    _Mem::find_object(objects, "pattern379"),
    _Mem::find_object(objects, "pattern1379"),
    _Mem::find_object(objects, "pattern2379"),
    _Mem::find_object(objects, "pattern12379"),
    _Mem::find_object(objects, "pattern479"),
    _Mem::find_object(objects, "pattern1479"),
    _Mem::find_object(objects, "pattern2479"),
    _Mem::find_object(objects, "pattern12479"),
    _Mem::find_object(objects, "pattern3479"),
    _Mem::find_object(objects, "pattern13479"),
    _Mem::find_object(objects, "pattern23479"),
    _Mem::find_object(objects, "pattern123479"),
    _Mem::find_object(objects, "pattern579"),
    _Mem::find_object(objects, "pattern1579"),
    _Mem::find_object(objects, "pattern2579"),
    _Mem::find_object(objects, "pattern12579"),
    _Mem::find_object(objects, "pattern3579"),
    _Mem::find_object(objects, "pattern13579"),
    _Mem::find_object(objects, "pattern23579"),
    _Mem::find_object(objects, "pattern123579"),
    _Mem::find_object(objects, "pattern4579"),
    _Mem::find_object(objects, "pattern14579"),
    _Mem::find_object(objects, "pattern24579"),
    _Mem::find_object(objects, "pattern124579"),
    _Mem::find_object(objects, "pattern34579"),
    _Mem::find_object(objects, "pattern134579"),
    _Mem::find_object(objects, "pattern234579"),
    _Mem::find_object(objects, "pattern1234579"),
    _Mem::find_object(objects, "pattern679"),
    _Mem::find_object(objects, "pattern1679"),
    _Mem::find_object(objects, "pattern2679"),
    _Mem::find_object(objects, "pattern12679"),
    _Mem::find_object(objects, "pattern3679"),
    _Mem::find_object(objects, "pattern13679"),
    _Mem::find_object(objects, "pattern23679"),
    _Mem::find_object(objects, "pattern123679"),
    _Mem::find_object(objects, "pattern4679"),
    _Mem::find_object(objects, "pattern14679"),
    _Mem::find_object(objects, "pattern24679"),
    _Mem::find_object(objects, "pattern124679"),
    _Mem::find_object(objects, "pattern34679"),
    _Mem::find_object(objects, "pattern134679"),
    _Mem::find_object(objects, "pattern234679"),
    _Mem::find_object(objects, "pattern1234679"),
    _Mem::find_object(objects, "pattern5679"),
    _Mem::find_object(objects, "pattern15679"),
    _Mem::find_object(objects, "pattern25679"),
    _Mem::find_object(objects, "pattern125679"),
    _Mem::find_object(objects, "pattern35679"),
    _Mem::find_object(objects, "pattern135679"),
    _Mem::find_object(objects, "pattern235679"),
    _Mem::find_object(objects, "pattern1235679"),
    _Mem::find_object(objects, "pattern45679"),
    _Mem::find_object(objects, "pattern145679"),
    _Mem::find_object(objects, "pattern245679"),
    _Mem::find_object(objects, "pattern1245679"),
    _Mem::find_object(objects, "pattern345679"),
    _Mem::find_object(objects, "pattern1345679"),
    _Mem::find_object(objects, "pattern2345679"),
    _Mem::find_object(objects, "pattern12345679"),
    _Mem::find_object(objects, "pattern89"),
    _Mem::find_object(objects, "pattern189"),
    _Mem::find_object(objects, "pattern289"),
    _Mem::find_object(objects, "pattern1289"),
    _Mem::find_object(objects, "pattern389"),
    _Mem::find_object(objects, "pattern1389"),
    _Mem::find_object(objects, "pattern2389"),
    _Mem::find_object(objects, "pattern12389"),
    _Mem::find_object(objects, "pattern489"),
    _Mem::find_object(objects, "pattern1489"),
    _Mem::find_object(objects, "pattern2489"),
    _Mem::find_object(objects, "pattern12489"),
    _Mem::find_object(objects, "pattern3489"),
    _Mem::find_object(objects, "pattern13489"),
    _Mem::find_object(objects, "pattern23489"),
    _Mem::find_object(objects, "pattern123489"),
    _Mem::find_object(objects, "pattern589"),
    _Mem::find_object(objects, "pattern1589"),
    _Mem::find_object(objects, "pattern2589"),
    _Mem::find_object(objects, "pattern12589"),
    _Mem::find_object(objects, "pattern3589"),
    _Mem::find_object(objects, "pattern13589"),
    _Mem::find_object(objects, "pattern23589"),
    _Mem::find_object(objects, "pattern123589"),
    _Mem::find_object(objects, "pattern4589"),
    _Mem::find_object(objects, "pattern14589"),
    _Mem::find_object(objects, "pattern24589"),
    _Mem::find_object(objects, "pattern124589"),
    _Mem::find_object(objects, "pattern34589"),
    _Mem::find_object(objects, "pattern134589"),
    _Mem::find_object(objects, "pattern234589"),
    _Mem::find_object(objects, "pattern1234589"),
    _Mem::find_object(objects, "pattern689"),
    _Mem::find_object(objects, "pattern1689"),
    _Mem::find_object(objects, "pattern2689"),
    _Mem::find_object(objects, "pattern12689"),
    _Mem::find_object(objects, "pattern3689"),
    _Mem::find_object(objects, "pattern13689"),
    _Mem::find_object(objects, "pattern23689"),
    _Mem::find_object(objects, "pattern123689"),
    _Mem::find_object(objects, "pattern4689"),
    _Mem::find_object(objects, "pattern14689"),
    _Mem::find_object(objects, "pattern24689"),
    _Mem::find_object(objects, "pattern124689"),
    _Mem::find_object(objects, "pattern34689"),
    _Mem::find_object(objects, "pattern134689"),
    _Mem::find_object(objects, "pattern234689"),
    _Mem::find_object(objects, "pattern1234689"),
    _Mem::find_object(objects, "pattern5689"),
    _Mem::find_object(objects, "pattern15689"),
    _Mem::find_object(objects, "pattern25689"),
    _Mem::find_object(objects, "pattern125689"),
    _Mem::find_object(objects, "pattern35689"),
    _Mem::find_object(objects, "pattern135689"),
    _Mem::find_object(objects, "pattern235689"),
    _Mem::find_object(objects, "pattern1235689"),
    _Mem::find_object(objects, "pattern45689"),
    _Mem::find_object(objects, "pattern145689"),
    _Mem::find_object(objects, "pattern245689"),
    _Mem::find_object(objects, "pattern1245689"),
    _Mem::find_object(objects, "pattern345689"),
    _Mem::find_object(objects, "pattern1345689"),
    _Mem::find_object(objects, "pattern2345689"),
    _Mem::find_object(objects, "pattern12345689"),
    _Mem::find_object(objects, "pattern789"),
    _Mem::find_object(objects, "pattern1789"),
    _Mem::find_object(objects, "pattern2789"),
    _Mem::find_object(objects, "pattern12789"),
    _Mem::find_object(objects, "pattern3789"),
    _Mem::find_object(objects, "pattern13789"),
    _Mem::find_object(objects, "pattern23789"),
    _Mem::find_object(objects, "pattern123789"),
    _Mem::find_object(objects, "pattern4789"),
    _Mem::find_object(objects, "pattern14789"),
    _Mem::find_object(objects, "pattern24789"),
    _Mem::find_object(objects, "pattern124789"),
    _Mem::find_object(objects, "pattern34789"),
    _Mem::find_object(objects, "pattern134789"),
    _Mem::find_object(objects, "pattern234789"),
    _Mem::find_object(objects, "pattern1234789"),
    _Mem::find_object(objects, "pattern5789"),
    _Mem::find_object(objects, "pattern15789"),
    _Mem::find_object(objects, "pattern25789"),
    _Mem::find_object(objects, "pattern125789"),
    _Mem::find_object(objects, "pattern35789"),
    _Mem::find_object(objects, "pattern135789"),
    _Mem::find_object(objects, "pattern235789"),
    _Mem::find_object(objects, "pattern1235789"),
    _Mem::find_object(objects, "pattern45789"),
    _Mem::find_object(objects, "pattern145789"),
    _Mem::find_object(objects, "pattern245789"),
    _Mem::find_object(objects, "pattern1245789"),
    _Mem::find_object(objects, "pattern345789"),
    _Mem::find_object(objects, "pattern1345789"),
    _Mem::find_object(objects, "pattern2345789"),
    _Mem::find_object(objects, "pattern12345789"),
    _Mem::find_object(objects, "pattern6789"),
    _Mem::find_object(objects, "pattern16789"),
    _Mem::find_object(objects, "pattern26789"),
    _Mem::find_object(objects, "pattern126789"),
    _Mem::find_object(objects, "pattern36789"),
    _Mem::find_object(objects, "pattern136789"),
    _Mem::find_object(objects, "pattern236789"),
    _Mem::find_object(objects, "pattern1236789"),
    _Mem::find_object(objects, "pattern46789"),
    _Mem::find_object(objects, "pattern146789"),
    _Mem::find_object(objects, "pattern246789"),
    _Mem::find_object(objects, "pattern1246789"),
    _Mem::find_object(objects, "pattern346789"),
    _Mem::find_object(objects, "pattern1346789"),
    _Mem::find_object(objects, "pattern2346789"),
    _Mem::find_object(objects, "pattern12346789"),
    _Mem::find_object(objects, "pattern56789"),
    _Mem::find_object(objects, "pattern156789"),
    _Mem::find_object(objects, "pattern256789"),
    _Mem::find_object(objects, "pattern1256789"),
    _Mem::find_object(objects, "pattern356789"),
    _Mem::find_object(objects, "pattern1356789"),
    _Mem::find_object(objects, "pattern2356789"),
    _Mem::find_object(objects, "pattern12356789"),
    _Mem::find_object(objects, "pattern456789"),
    _Mem::find_object(objects, "pattern1456789"),
    _Mem::find_object(objects, "pattern2456789"),
    _Mem::find_object(objects, "pattern12456789"),
    _Mem::find_object(objects, "pattern3456789"),
    _Mem::find_object(objects, "pattern13456789"),
    _Mem::find_object(objects, "pattern23456789"),
    _Mem::find_object(objects, "pattern123456789"),
  };

  for (size_t i = 0; i < N_FOVEA_PATTERNS; ++i) {
    if (!fovea_patterns_[i])
      return false;
  }

  return true;
}

void VideoScreen::move_eye(int delta_x, int delta_y, int& actual_delta_x, int& actual_delta_y) {
  // Keep the full fovea within the boundaries of the screen.
  int new_x = max(1, min(width_ -  2, fovea_x_ + delta_x));
  int new_y = max(1, min(height_ - 2, fovea_y_ + delta_y));

  // TODO: Need a critical section?
  actual_delta_x = new_x - fovea_x_;
  actual_delta_y = new_y - fovea_y_;
  fovea_x_ = new_x;
  fovea_y_ = new_y;
}

Code* VideoScreen::get_fovea_pattern() const {
  // move_eye already made sure we are within bounds.
  // Get the bit pattern, same order as in fovea-patterns.replicode .
  bool fovea[] {
    get_screen(fovea_x_ - 1, fovea_y_ + 1),
    get_screen(fovea_x_,     fovea_y_ + 1),
    get_screen(fovea_x_ + 1, fovea_y_ + 1),
    get_screen(fovea_x_ - 1, fovea_y_),
    get_screen(fovea_x_,     fovea_y_),
    get_screen(fovea_x_ + 1, fovea_y_),
    get_screen(fovea_x_ - 1, fovea_y_ - 1),
    get_screen(fovea_x_,     fovea_y_ - 1),
    get_screen(fovea_x_ + 1, fovea_y_ - 1),
  };
  int index = 0;
  for (int i = 8; i >= 0; --i) {
    index <<= 1;
    if (fovea[i])
      index |= 1;
  }

  // TODO: Cache this.
  return fovea_patterns_[index];
}

}
