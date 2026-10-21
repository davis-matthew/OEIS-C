/*
 * A258623  Number of length n 1..(n+1) arrays with every leading partial sum
 *          divisible by 2, 3, 5 or 7.
 * https://oeis.org/A258623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length‑n arrays with entries 1 … (n+1) such
 *            that every leading partial sum is divisible by 2, 3, 5 or 7
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   init_allowed_residues
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/init_allowed_residues.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;
    }

    /* initialise allowed residues once */
    static bool inD[210];
    static int Dcnt = 0;
    static bool init = false;
    if (!init) {
        init_allowed_residues(inD, &Dcnt);
        init = true;
    }

    /* pre‑compute how many numbers in 1 … (n+1) give each residue */
    idx_t N = n + (idx_t)1;            /* largest allowed increment */
    seq_t incMask[210];
    for (int d = 0; d < 210; ++d) {
        if (d == 0) {
            incMask[d] = N / 210;
        } else if ((idx_t)d <= N) {
            incMask[d] = (N - (idx_t)d) / 210 + (seq_t)1;
        } else {
            incMask[d] = 0;
        }
    }

    /* DP arrays */
    seq_t ways[210] = {0};
    seq_t next[210];
    ways[0] = (seq_t)1;   /* starting from sum 0, which is divisible by all the primes */

    /* iterate over the n positions */
    for (idx_t step = 0; step < n; ++step) {
        /* clear next */
        for (int t = 0; t < 210; ++t) {
            next[t] = (seq_t)0;
        }

        /* convolution step: for each current residue s, add
           contributions via all possible increments d with weight incMask[d]. */
        for (int s = 0; s < 210; ++s) {
            seq_t cur = ways[s];
            for (int d = 0; d < 210; ++d) {
                seq_t cnt = incMask[d];
                if (!cnt) {
                    continue;
                }
                int t = s + d;
                if (t >= 210) {
                    t -= 210;
                }
                if (!inD[t]) {
                    continue;
                }
                next[t] += cur * cnt;
            }
        }

        /* move to next step */
        for (int t = 0; t < 210; ++t) {
            ways[t] = next[t];
        }
    }

    /* sum over all allowed residues gives the final count */
    seq_t result = (seq_t)0;
    for (int t = 0; t < 210; ++t) {
        result += ways[t];
    }
    return result;
}
