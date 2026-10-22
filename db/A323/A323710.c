/*
 * A323710  a(n) is the symmetrical of n via transport of structure from
 *          binary trees, where the binary tree of n is built as follows:
 *          create a root with value n and recursively apply the rule {write
 *          node's value as (2^c)*(2*k+1); if c>0, create a left child with
 *          value c; if k>0, create a right child with va...
 * https://oeis.org/A323710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   let n = 2^c · (2k+1) with c≥0, k≥0. Then a(0) = 0 (used for
 *            missing subtrees) a(n) = 2^{a(k)} · (2·a(c) + 1) This is the
 *            involution a(n) = g(r(f(n))) from OEIS A323710
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   pow_seq_large
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/pow_seq_large.h"

static seq_t *cache = NULL;          /* memoisation array */
static idx_t  cache_size = 0;        /* current capacity */

/* grow the cache to accommodate indices up to n */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_size)
        return;
    idx_t new_size = (n + 1) * 2;                /* double to reduce reallocs */
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    cache = tmp;
    for (idx_t i = cache_size; i < new_size; ++i)
        cache[i] = (seq_t)0;                    /* 0 = not yet computed */
    cache_size = new_size;
}

/* recursive helper; uses memoisation */
static seq_t a_rec(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                      /* base for missing subtrees */

    ensure_cache(n);
    if (cache[n] != (seq_t)0)                 /* already computed? */
        return cache[n];

    /* decompose n = 2^c * (2k+1) */
    idx_t m = n;
    idx_t c = 0;
    while ((m & 1) == 0) {                    /* count trailing zeros */
        ++c;
        m >>= 1;
    }
    idx_t k = (m - 1) / 2;                    /* k≥0 */

    seq_t left  = a_rec(k);                    /* a(k)  */
    seq_t right = a_rec(c);                    /* a(c)  */

    /* compute 2^{a(k)} and (2·a(c)+1) */
    seq_t pow2_left = pow_seq_large((seq_t)2, left);
    seq_t two_right_plus1 = right * (seq_t)2 + (seq_t)1;

    seq_t result = pow2_left * two_right_plus1;
    cache[n] = result;
    return result;
}

/* external entry point required by the test harness */
seq_t a(idx_t n)
{
    return a_rec(n);
}
