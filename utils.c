//=======================================================================
// Copyright abeinoe 2015.
// Distributed under the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

short checkBox(const unsigned char *box)
{
    short i, j, k, l;

    l = 0;
    for(i=0; i < 256; i++){
        k = 0;
        for(j=0; j < 256; j++){
            if(box[j] == (unsigned char) i)
                k++;
        }
        if(k > 1){
            l++;
        } else if(k < 1){
            l++;
        }
    }

    if(l == 0){
        return 0;
    }

    return 1;
}
