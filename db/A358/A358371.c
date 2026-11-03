/*
 * A358371  Number of leaves in the n-th standard ordered rooted tree.
 * https://oeis.org/A358371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of leaves in the n-th standard ordered rooted tree
 *            computed by recursively interpreting the (n-1)-th composition in
 *            standard ordered composition sequence (graded
 *            reverse‑lexicographic) as the root parts and summing the leaves
 *            of the subtrees indexed by those parts. The implementatio...
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Validated: partial, pass, 86 terms through n = 86 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t leaf_value(idx_t n)
{
    /* Small cache for indices that actually appear as sub‑tree indices. */
    enum { CACHE_SIZE = 80 };                 /* > maximal weight (~14) */
    static seq_t cache[CACHE_SIZE] = {0};

    if (n == 1)
        return 1;                            /* leaf itself */

    if (n < CACHE_SIZE && cache[n] != 0)
        return cache[n];

    seq_t sum = 0;

    /* rank of the composition for this tree (0-indexed) */
    idx_t r = n - 1;

    /* Determine the weight w of the composition that contains rank r.
       Compositions of weight w occupy ranks [2^(w-1) , 2^w-1] (w≥1). */
    idx_t w = 0;
    idx_t pow = 1;                            /* 2^(w-1) */
    while (pow <= r) {                       /* loop until pow > r */
        ++w;
        pow <<= 1;                            /* pow = 2^w */
    }

    idx_t prev_pow = pow >> 1;                /* 2^(w-1) */
    idx_t off      = r - prev_pow;            /* offset inside the weight‑w block */

    /* Build the composition parts a1,a2,… using the decomposition algorithm. */
    idx_t rem   = w;                          /* remaining weight to process */
    idx_t rank  = off;                        /* current offset inside rem */

    while (rem > 0) {
        /* Find the first part a1 such that rank lies in its block. */
        for (idx_t a1 = rem; a1 >= 1; --a1) {
            idx_t m     = rem - a1;          /* weight of the rest */
            idx_t count = (m == 0) ? 1
                           : ((idx_t)1 << (m - 1)); /* compositions of weight m */

            if (rank < count) {
                sum += leaf_value(a1);        /* recurse on the sub‑tree index a1 */
                rem  = m;                     /* next sub‑composition weight  */
                /* rank stays unchanged */
                break;
            } else {
                rank -= count;                /* skip this block */
            }
        }
    }

    if (n < CACHE_SIZE)
        cache[n] = sum;

    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* undefined for offset 0, but keep safe */
    return leaf_value(n);
}
