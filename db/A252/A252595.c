/*
 * A252595  Numbers m such that m^3 is the sum of n-th group in partition of
 *          sequence of natural numbers.
 * https://oeis.org/A252595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = integer cube root of the sum of the n-th group in the *
 *            partition of the natural numbers into consecutive groups whose
 *            sums are * perfect cubes
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Validated: partial, pass, 6 terms through n = 6 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* Floor cube root by binary search */
static inline seq_t floor_cube_root(seq_t x)
{
    seq_t lo = 0, hi = 1;
    /* raise hi until hi^3 > x */
    while (ipow_i32(hi, 3) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid3 = ipow_i32(mid, 3);
        if (mid3 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n) {
    /* Static state to allow arbitrary-order calls. */
    static seq_t *seq = NULL;          /* cached sequence terms */
    static size_t seq_len = 0;         /* number of terms stored */
    static size_t seq_cap = 0;         /* allocated capacity of seq */
    static seq_t   sum = 0;            /* running sum of current group */
    static idx_t   next = 1;           /* next natural number to process */

    if (n == 0) return 0; /* undefined for offset 1, but harmless */

    /* Initialise cache on first call. */
    if (!seq) {
        seq_cap = 16;
        seq = mem_new(seq_t, seq_cap);
    }

    /* Generate terms until we have at least n. */
    while (seq_len < n) {
        sum += (seq_t)next;

        /* Check if the running sum is a perfect cube. */
        seq_t r = floor_cube_root(sum);
        seq_t cube = r * r * r;
        if (cube == sum) {
            /* Store the cube root as the next sequence term. */
            if (seq_len == seq_cap) {
                seq_cap = seq_cap ? seq_cap * 2 : 16;
                seq = mem_resize(seq_t, seq, seq_cap);
            }
            seq[seq_len++] = r;
            sum = 0; /* start a new group */
        }

        /* Advance to the next integer. */
        ++next;
    }

    return seq[n - 1];
}
