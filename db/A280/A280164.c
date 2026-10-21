/*
 * A280164  Number of integer pairs (k, b) such that n and k*n are anagrams in
 *          base-b (k > 1).
 * https://oeis.org/A280164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 713, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of integer pairs (k,b) such that n and k*n are  
   anagrams in base‑b with k > 1.  The implementation iterates over all  
   bases 2 ≤ b ≤ n, filters the candidate multipliers k using the congruence  
   (b‑1) | n·(k‑1), and then checks whether the digit multisets of n and k*n  
   coincide.  All arithmetic is carried out in the arbitrary‑precision type  
   seq_t provided by the build.  No tables of values are used. */
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd.h"

#define MAX_BASE 10001               /* supports n ≤ 10000 */

/* digit count arrays for the current base */
static int cnt[MAX_BASE];
static int tmpcnt[MAX_BASE];

seq_t a(idx_t n)
{
    /* n is at least 1 for all requested indices; a(1)=0 naturally. */
    seq_t total = 0;
    seq_t nn = (seq_t)n;

    for (idx_t b = 2; b <= n; ++b) {
        /* number of digits of n in base b */
        idx_t temp = n;
        int d = 0;
        while (temp) {
            ++d;
            temp /= b;
        }

        /* compute b^d as seq_t */
        seq_t powBd = 1;
        for (int i = 0; i < d; ++i) powBd *= (seq_t)b;

        /* digit multiset of n in base b */
        int limit = (int)b;               /* b ≤ n ≤ MAX_BASE-1 */
        memset(cnt, 0, limit * sizeof(int));
        seq_t tempN = nn;
        while (tempN) {
            int digit = (int)(tempN % (seq_t)b);
            ++cnt[digit];
            tempN /= b;
        }

        /* step size for k: k = 1 + t * ((b-1)/gcd(n,b-1)) */
        idx_t mInt = (idx_t)b - 1;
        seq_t step_seq = (seq_t)mInt / gcd((seq_t)mInt, nn);
        idx_t step = (idx_t)step_seq;

        /* iterate over admissible k values */
        for (idx_t t = 1; ; ++t) {
            seq_t k = (seq_t)1 + (seq_t)t * step_seq;
            seq_t prod = k * nn;
            if (prod >= powBd) break;

            /* digit multiset of prod in base b */
            memset(tmpcnt, 0, limit * sizeof(int));
            seq_t tempP = prod;
            while (tempP) {
                int digit = (int)(tempP % (seq_t)b);
                ++tmpcnt[digit];
                tempP /= b;
            }

            /* compare multisets */
            if (memcmp(cnt, tmpcnt, limit * sizeof(int)) == 0)
                ++total;
        }
    }
    return total;
}
