#include <math.h>
#include <stdio.h>
#include <string.h>

#define max(a,b)(((a)>(b))?(a):(b))

/*
This script is used to show that minimal superpermutations on 5 symbols have 153 characters (it can also compute the length of minimal superpermutations on 3 or 4 symbols).
More specifically, it computes the values in the table from the following blog post:
http://www.njohnston.ca/2014/08/all-minimal-superpermutations-on-five-symbols-have-been-found/

Author: Nathaniel Johnston (nathaniel@njohnston.ca), based on an algorithm by Benjamin Chaffin
Version: 1.00
Last Updated: Aug. 13, 2014
*/

unsigned short int curstr[1000], max_perm, max_total_perm, mperm_res[1000], n, tot_bl;
unsigned long int cur_perms[1000];

void fillStr(short int pos, short int pfound);
void clearPerms();
unsigned long int fac(int k);

int main()
{
    unsigned short int j0;

    printf("This tool will try to find the length of the shortest superpermutation on n symbols. Please enter n: ");
    scanf("%d",&n);

    mperm_res[0] = n;
    max_total_perm = 1;

    for(tot_bl=1; tot_bl<=10000; tot_bl++){
        max_perm = 1;

        // fill the first n entries of the string with 1...n
        clearPerms();
        for(j0=0; j0<n; j0++){
            curstr[j0] = j0+1;
            cur_perms[0] = 10*cur_perms[0] + j0+1;
        }

        fillStr(n,1);

        printf("%d wasted characters: at most %d permutations\n",tot_bl,max_perm);
        mperm_res[tot_bl] = max_perm;

        if(max_perm >= fac(n) / 2){
            unsigned short int length = (fac(n)/2+tot_bl+n-1) * 2 - 1;
            printf("\n-----\nDONE!\n-----\n\nMinimal superpermutations on %d symbols have %d wasted characters and a length of %d.\n",n,tot_bl,length);
            break;
        }
    }

    getchar();
    return 0;
}

// this function recursively fills the string
void fillStr(short int pos, short int pfound)
{
    unsigned short int i1, j1, k1, newperm, repeated, repeated2;
    unsigned long int tperm, tperm2;

    for(j1=1; j1<=n; j1++){
        // there is never any benefit to having 2 of the same character next to each other
        if(j1 != curstr[pos-1]){
            curstr[pos] = j1;

            // now check to see if this contributes a new permutation or not
            // first construct a decimal number representing the permutation
            tperm = 0;
            for(k1=pos-n+1; k1<=pos; k1++){
                tperm = 10*tperm + curstr[k1];
            }

            // construct a decimal number for the reverse of the permutation
            tperm2 = 0;
            for(k1=pos; k1>=pos-n+1; k1--){
                tperm2 = 10*tperm2 + curstr[k1];
            }

            // now check that the n most recent characters form a permutation (i.e., don't contain any repeated symbols)
            newperm = 1;
            for(k1=pos-n+1; k1<=pos; k1++){
                for(i1=k1+1; i1<=pos; i1++){
                    if(curstr[k1] == curstr[i1]){
                        newperm = 0;
                        break;
                    }
                }
            }

            // now actually check if we've already found it
            repeated = 0;
            repeated2 = 0;
            if(newperm == 1){
                for(k1=0; k1<pfound; k1++){
                    if(tperm == cur_perms[k1]){
                        newperm = 0;
                        repeated = 1;
                        break;
                    }
                }

                for(k1=0; k1<pfound; k1++){
                    if(tperm2 == cur_perms[k1]){
                        repeated2 = 1;
                        break;
                    }
                }
            }

            // if the permutation already appears forwards or backwards in the
            // string then discard this candidate string by not recursing
            if (repeated || repeated2) {
              continue;
            }

            // now go to the next level of the recursion
            if(newperm == 1){
                max_perm = max(max_perm,pfound+1);

                if (max_perm > max_total_perm) {
                  max_total_perm = max_perm;
                  for(k1=0; k1<pos+1; k1++) {
                    printf("%d", curstr[k1]);
                  }
                  printf("\n\n");
                }

                cur_perms[pfound] = tperm;
                fillStr(pos+1, pfound+1);
                cur_perms[pfound] = 0;

            // the quantity pos - pfound - n + 1 is the number of already-used blanks
            }else if(pos - pfound - n + 1 < tot_bl){
                if(mperm_res[tot_bl - pos + pfound + n - 2] + pfound > max_perm){
                    fillStr(pos+1, pfound);
                }
            }
        }
    }
}

// this function just wipes out all currently-found permutations (used when we increase the number of wasted characters that we're allowing)
void clearPerms()
{
    unsigned short int j2;

    for(j2=0; j2<1000; j2++){
        cur_perms[j2] = 0;
    }
}

// this function computes the factorial of a number
unsigned long int fac(int k)
{
    if(k <= 1){
        return 1;
    }else{
        return k*fac(k-1);
    }
}
