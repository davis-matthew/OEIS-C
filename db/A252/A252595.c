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
 * Helpers:   floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/floor_cube_root.h"

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
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
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
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_len++] = r;
            sum = 0; /* start a new group */
        }

        /* Advance to the next integer. */
        ++next;
    }

    return seq[n - 1];
}
