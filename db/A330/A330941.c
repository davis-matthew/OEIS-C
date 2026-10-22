/*
 * A330941  a(n) is the greatest value whose binary representation can be
 *          obtained by interleaving (or shuffling) two copies of the binary
 *          representation of n.
 * https://oeis.org/A330941
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = maximum interleaving of two copies of the binary
 *            representation of n (lexicographically largest shuffle)
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   bit_length_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/bit_length_u64.h"

seq_t a(idx_t n)
{
    if (n == 0)   /* binary 0 → interleaving of two zeros gives 0 */
        return 0;

    int L = bit_length_u64(n);          /* length of the binary string of n */
    unsigned char bits[64];             /* bits[0] = most significant, bits[L‑1] = least */

    for (int pos = 0; pos < L; ++pos)
        bits[pos] = (n >> (L - 1 - pos)) & 1U;

    seq_t res = 0;
    int i = 0, j = 0;                   /* indices into the two copies */

    for (int step = 0; step < 2 * L; ++step) {
        bool chooseA;
        if (i == L)                     /* copy A exhausted → pick from B */
            chooseA = false;
        else if (j == L)                /* copy B exhausted → pick from A */
            chooseA = true;
        else {
            /* Compare the remaining suffixes bits[i..] and bits[j..]. */
            int di = i, dj = j;
            while (di < L && dj < L && bits[di] == bits[dj]) {
                ++di;
                ++dj;
            }
            if (di == L && dj == L)     /* identical suffixes, tie → pick A */
                chooseA = true;
            else if (di == L)           /* A exhausted sooner → pick B */
                chooseA = false;
            else if (dj == L)           /* B exhausted sooner → pick A */
                chooseA = true;
            else                        /* decide by first differing bit */
                chooseA = bits[di] > bits[dj];
        }

        if (chooseA) {
            res = (res << 1) | (seq_t)bits[i++];
        } else {
            res = (res << 1) | (seq_t)bits[j++];
        }
    }

    return res;
}
