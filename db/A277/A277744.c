/*
 * A277744  Fixed point of the morphism 1->23, 2->14, 3->21, 4->56, 5->63,
 *          6->54; starting with a(1) = 1.
 * https://oeis.org/A277744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; * For n>1: let k = n/2. If n is even then * a(n) =
 *            f(a(k)); * else a(n) = g(a(k)); * where the two component tables
 *            are * f[1]=4, f[2]=3, f[3]=6, f[4]=1, f[5]=4, f[6]=3 * g[1]=2,
 *            g[2]=5, g[3]=1, g[4]=6, g[5]=2, g[6]=5 * (f and g are defined
 *            for numerals 1‑6, index 0 is unused)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* mapping tables for the two children of each symbol */
static const seq_t f[7] = { 0, 4, 3, 6, 1, 4, 3 };   /* f[1..6] */
static const seq_t g[7] = { 0, 2, 5, 1, 6, 2, 5 };   /* g[1..6] */

seq_t a(idx_t n)
{
    if (n == 1)
        return 1;

    /* recurse to the parent index */
    idx_t parent_idx = n >> 1;                /* parent = n/2   */
    seq_t parent_val = a(parent_idx);         /* value at parent */

    /* choose the appropriate child mapping */
    if ((n & 1ULL) == 0)
        return f[(size_t)parent_val];
    else
        return g[(size_t)parent_val];
}
