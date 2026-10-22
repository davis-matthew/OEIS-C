/*
 * A378298  Number of solutions that satisfy the congruence: i^2 == j^2 (mod
 *          n) with 1 <= i < j <= n.
 * https://oeis.org/A378298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum over residues r of binomial(count[r],2) where
 *            count[r] = #{ i ∈ [1..n] : i^2 mod n = r }
 * Range:     u8 n <= 95, u16 n <= 5039, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n <= 1) return 0;

    /* Allocate an array to hold the counts of each quadratic residue. */
    idx_t *count = (idx_t *)malloc(n * sizeof(idx_t));
    if (!count) return 0;            /* memory failure – return 0 (not expected) */

    for (idx_t i = 0; i < n; ++i)
        count[i] = 0;

    /* Count the residues. */
    for (idx_t i = 1; i <= n; ++i) {
        idx_t r = (i * i) % n;
        ++count[r];
    }

    /* Sum binomial(count,2) over all residues. */
    idx_t result = 0;
    for (idx_t r = 0; r < n; ++r) {
        idx_t c = count[r];
        if (c > 1)
            result += c * (c - 1) / 2;
    }

    free(count);
    return (seq_t)result;
}
