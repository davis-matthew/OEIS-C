/*
 * A351524  Number of powers of 11 modulo n.
 * https://oeis.org/A351524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of distinct residues of 11^k modulo n (k ≥ 0)
 * Range:     u8 n <= 275, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n = 0, never called */
    /* Allocate a boolean array marked for each possible residue modulo n */
    bool *seen = (bool *)malloc((size_t)n * sizeof(bool));
    if (!seen) return 0;             /* fallback on allocation failure */
    for (idx_t i = 0; i < n; ++i) seen[i] = false;

    seq_t modn = (seq_t)n;
    seq_t current = 1 % modn;        /* 11^0 mod n */
    seq_t step = (seq_t)11 % modn;   /* 11 mod n */
    seq_t count = 0;

    /* Iterate until a residue repeats */
    while (!seen[(uint64_t)current]) {
        seen[(uint64_t)current] = true;
        ++count;
        current = (current * step) % modn;
    }

    free(seen);
    return count;
}
