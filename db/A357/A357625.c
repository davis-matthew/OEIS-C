/*
 * A357625  Numbers k such that the k-th composition in standard order has
 *          half-alternating sum 0.
 * https://oeis.org/A357625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th nonnegative integer k such that the k‑th
 *            composition (standard order) has half‑alternating sum 0
 * Range:     u8 n <= 39, u16 n <= 7762, u32 n <= 7762, u64 n <= 7762,
 *            u128 n <= 7762, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* The function uses the public types seq_t and idx_t supplied by the
   build system.  idx_t is an unsigned 64‑bit type; seq_t is an unsigned
   integer type of variable size (possibly an arbitrary‑precision
   class).  The implementation keeps a static cache of found k's and
   generates more on demand. */

seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* safety for bad input (offset is 1) */

    static seq_t *list = NULL;
    static size_t list_len = 0, list_cap = 0;
    static idx_t next_k = 0;            /* next k to test */

    /* Expand the list until it contains at least n elements. */
    while (list_len < n) {
        idx_t k = next_k;               /* candidate k */

        /* ----------------------------------------------------
           1. Find positions of 1‑bits in the reversed binary
              representation of k.  Positions are 1‑based and
              recorded from least significant bit upwards.
           ---------------------------------------------------- */
        int pos[64];
        int m = 0;                      /* number of 1‑bits */
        idx_t tmp = k;
        int i = 0;
        while (tmp) {
            if (tmp & 1ULL)
                pos[m++] = i + 1;       /* 1‑based position */
            tmp >>= 1;
            ++i;
        }

        /* ----------------------------------------------------
           2. Compute the first differences (prepend 0) to obtain
              the parts of the composition before the final reverse.
           ---------------------------------------------------- */
        int diff[64];
        int prev = 0;
        for (int j = 0; j < m; ++j) {
            diff[j] = pos[j] - prev;
            prev = pos[j];
        }

        /* ----------------------------------------------------
           3. Compute the half‑alternating sum on the reversed
              list of parts.  The sign pattern is
                 +, +, -, -, +, +, -, - , ...
              i.e. sign = +1 for r%4∈{0,1}, -1 for r%4∈{2,3},
              where r is the index in the reversed list
              (starting at 0).
           ---------------------------------------------------- */
        long long hsum = 0;
        for (int j = m - 1, r = 0; j >= 0; --j, ++r) {
            long long s = (r / 2) & 1 ? -1 : 1;
            hsum += s * (long long)diff[j];
        }

        /* If the half‑alternating sum is zero, record k. */
        if (hsum == 0) {
            if (list_len == list_cap) {
                list_cap = list_cap ? (list_cap << 1) : 32;
                list = (seq_t *)realloc(list, list_cap * sizeof(seq_t));
            }
            list[list_len++] = (seq_t)k;
        }

        ++next_k;
    }

    return list[n - 1];
}
