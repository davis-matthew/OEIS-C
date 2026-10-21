/*
 * A280338  Number of sizes of remainder sets for n, for any natural number c,
 *          given natural number b in (b^c) mod n.
 * https://oeis.org/A280338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ size of orbit of multiplication-by-b modulo n |
 *            b∈[0,n-1] }| The orbit of b is the set { b^c mod n : c ≥ 1 }
 *            obtained by iterating x → (x·b) mod n. Its size is the number of
 *            distinct residues visited before a value repeats. a(n) records
 *            how many different such sizes occur among all resi...
 * Range:     u8 n <= 500, u16 n <= 500, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* safety, though n≥1 in OEIS */
    if (n == (idx_t)1) return (seq_t)1;   /* special case: only 0 exists */

    /* allocate arrays used in the per-base simulation */
    unsigned char *seen  = (unsigned char*)calloc((size_t)n + 1, sizeof(unsigned char));
    idx_t *mark = (idx_t*)calloc((size_t)n, sizeof(idx_t));
    if (!seen || !mark) { return (seq_t)0; }   /* allocation failed */

    idx_t cur_mark = 1;          /* stamp for the current base */
    for (idx_t b = 0; b < n; ++b) {
        idx_t x = b % n;        /* starting value of the orbit */
        idx_t steps = 0;        /* number of distinct values seen */

        /* iterate until a value repeats in this orbit */
        while (mark[x] != cur_mark) {
            mark[x] = cur_mark;
            ++steps;
            x = (x * b) % n;
        }

        if (steps <= n) seen[steps] = 1;   /* record that this size appeared */

        ++cur_mark;                 /* move to a new stamp for next base */
        if (cur_mark == 0) {        /* unlikely for n ≤ 10 000, but safe */
            memset(mark, 0, (size_t)n * sizeof(idx_t));
            cur_mark = 1;
        }
    }

    /* count how many distinct sizes were seen */
    seq_t count = (seq_t)0;
    for (idx_t i = 1; i <= n; ++i)
        if (seen[i]) ++count;

    free(seen);
    free(mark);
    return count;
}
