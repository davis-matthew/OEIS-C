/*
 * A397583  Number of ways to tile a 2 X n strip with one corner missing,
 *          using squares, dominoes, and T-shaped tetrominoes.
 * https://oeis.org/A397583
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 2*a(n-2) - a(n-3) - a(n-4) Initial values:
 *            a(0)=0, a(1)=1, a(2)=3, a(3)=11
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers required for C11 and C++17 compatibility */
#include <stddef.h>   /* size_t */
#include <stdint.h>   /* uint64_t */
#include <stdbool.h>  /* bool */

/*  Static cache for n up to 10000; larger n are computed on the fly.
    The size 10001 guarantees space for a(0) … a(10000). */
static seq_t cache[10001];
static idx_t up_to = 0;        /* highest index already computed in cache */

/* Initialise the cache with the first four terms. */
static void init_cache(void)
{
    if (up_to != 0)
        return;               /* already initialised */
    cache[0] = 0;
    cache[1] = 1;
    cache[2] = 3;
    cache[3] = 11;
    up_to   = 3;
}

/* Return the n‑th term of OEIS A397583. */
seq_t a(idx_t n)
{
    init_cache();

    /* If n is inside the cached range, return it directly. */
    if (n <= up_to)
        return cache[n];

    /* If n is within the pre‑allocated cache, extend it. */
    if (n <= 10000) {
        for (idx_t i = up_to + 1; i <= n; ++i) {
            cache[i] =
                cache[i-1] * (seq_t)3
              + cache[i-2] * (seq_t)2
              - cache[i-3]
              - cache[i-4];
        }
        up_to = n;
        return cache[n];
    }

    /* For n > 10000, compute iteratively without caching.  */
    seq_t a0 = cache[0];
    seq_t a1 = cache[1];
    seq_t a2 = cache[2];
    seq_t a3 = cache[3];

    if (n == 0) return a0;
    if (n == 1) return a1;
    if (n == 2) return a2;
    if (n == 3) return a3;

    for (idx_t i = 4; i <= n; ++i) {
        seq_t an = a3 * (seq_t)3 + a2 * (seq_t)2 - a1 - a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return a3;
}
