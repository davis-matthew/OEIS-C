/*
 * A394348  Number of rectangular plane partitions of n with exactly 3 rows
 *          and with rows and columns strictly decreasing.
 * https://oeis.org/A394348
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of rectangular plane partitions of n with
 *            exactly 3 rows and strictly decreasing rows and columns. It is
 *            obtained from the generating function:
 * Range:     u8 n <= 28, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 57 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  G(q) = Sum_{k>=1} q^(3*k*(k+3)/2) * Prod_{i=1..3} Prod_{j=1..k} 1/(1-q^(i+j-1)) */

#include <stdlib.h>
#include "../utils/mem.h"

static idx_t min_sum_of_k(idx_t k)
{
    /* 3*k*(k+3)/2 fits in idx_t for k up to about 80 (needed for n<=10000) */
    return (idx_t)((seq_t)3 * (seq_t)k * ((seq_t)k + 3) / 2);
}

/* Main function requested by the build system */
seq_t a(idx_t n)
{
    /* Allocate the sequence array 0 .. n */
    seq_t *seq = mem_new(seq_t, (size_t)(n + 1));
    if (!seq) exit(1);   /* out of memory */

    for (idx_t k = 1; ; ++k)
    {
        idx_t base = min_sum_of_k(k);
        if (base > n)
            break;            /* no more shapes contribute to n */

        idx_t max_extra = n - base;
        size_t ne = (size_t)(max_extra + 1);

        /* ways[x] = number of ways to add extra sum x to this shape */
        seq_t *ways = mem_new(seq_t, ne);
        if (!ways) { mem_free(seq); exit(1); }
        ways[0] = 1;          /* one way to reach 0 extra */

        /* For each possible weight m = i + j - 1 (1 <= i <= 3, 1 <= j <= k) */
        for (int m = 1; m <= (int)(k + 2); ++m)
        {
            /* Count how many (i, j) pairs give this weight m */
            int cnt = 0;
            for (int i = 1; i <= 3; ++i)
            {
                int j = m - i + 1;
                if (j >= 1 && j <= (int)k)
                    ++cnt;
            }

            /* For each copy of this weight, perform an unbounded coin change step */
            for (int pass = 0; pass < cnt; ++pass)
            {
                for (size_t d = (size_t)m; d < ne; ++d)
                    ways[d] += ways[d - m];
            }
        }

        /* Add the contributions of this shape to the global sequence */
        for (size_t e = 0; e < ne; ++e)
            seq[(size_t)base + e] += ways[e];

        mem_free(ways);
    }

    seq_t result = seq[n];
    mem_free(seq);
    return result;
}
