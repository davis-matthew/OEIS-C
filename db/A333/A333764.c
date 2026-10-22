/*
 * A333764  Numbers k such that the k-th composition in standard order is a
 *          co-necklace.
 * https://oeis.org/A333764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: enumerate positive integers k starting from 1.
   For each k, take the reversed binary representation of k.
   The positions (1‑based) of the 1 bits are taken, 0 is prepended,
   first differences of consecutive positions give the parts of a
   composition, and the resulting list of parts is reversed.
   The composition is a co‑necklace iff it is lexicographically
   greater than or equal to every one of its cyclic rotations.
   The function returns the nth such k (n ≥ 1). */

#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t count = 0;        /* number of co‑necklaces found so far   */
    idx_t k = 0;            /* current integer being tested         */

    while (count < n) {
        ++k;                /* examine the next integer              */

        /* Generate the composition corresponding to k. */
        idx_t kcopy = k;
        int pos = 1;       /* positions are 1‑based in the reversed bit string */
        int prev = 0;      /* position of the previous 1 (or the prepended 0)   */
        int parts[64];
        int len = 0;

        while (kcopy) {
            if (kcopy & 1U) {
                int diff = pos - prev;   /* part length   */
                parts[len++] = diff;
                prev = pos;
            }
            kcopy >>= 1;
            ++pos;
        }

        /* Reverse the parts list to obtain the final composition. */
        for (int i = 0; i < len / 2; ++i) {
            int t = parts[i];
            parts[i] = parts[len - 1 - i];
            parts[len - 1 - i] = t;
        }

        /* Check whether the composition is a co‑necklace. */
        bool co_necklace = true;
        for (int r = 1; r < len && co_necklace; ++r) {
            for (int i = 0; i < len; ++i) {
                int a = parts[i];
                int b = parts[(i + r) % len];
                if (a < b) {          /* rotation greater → not a co‑necklace */
                    co_necklace = false;
                    break;
                }
                if (a > b)            /* rotation smaller → stop early   */
                    break;
            }
        }

        if (co_necklace)
            ++count;                /* found one more co‑necklace */
    }

    return (seq_t)k;
}
