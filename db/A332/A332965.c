/*
 * A332965  a(n) is the number of distinct values in the sequence s defined by
 *          s(1) = 0 and for any k > 0, s(k+1) = (s(k)^2+1) mod n.
 * https://oeis.org/A332965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2400, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = the number of distinct values in the sequence
   s defined by s(1)=0 and s(k+1) = (s(k)^2+1) mod n. */
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Special case: n == 0 is outside the OEIS offset; return 0. */
    if (n == 0) return (seq_t)0;

    /* Allocate a visited flag array for all residues modulo n. */
    size_t sz = (size_t)n;
    bool *vis = (bool *)calloc(sz, sizeof(bool));
    if (!vis) return (seq_t)0;  /* allocation failure */

    idx_t s = 0;          /* current term */
    idx_t count = 0;      /* distinct count */

    while (!vis[(size_t)s]) {
        vis[(size_t)s] = true;
        ++count;
        s = (s * s + 1) % n;   /* next term, stays in [0, n-1] */
    }

    free(vis);
    return (seq_t)count;
}
