/*
 * A249306  Denominators A027642(n) of Bernoulli numbers except for a(4*k+5)=2
 *          instead of 1.
 * https://oeis.org/A249306
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 35, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A249306 – Denominators of Bernoulli numbers with a(4k+5)=2 instead of 1.
   For n==0      : a(0)=1
   For odd n      : a(2k+1)=2 if k even, =1 if k odd                (alternating 2,1,2,1,…)
   For even n>0   : a(n)=Π{p prime | (p-1) divides n}                 (product of such primes) */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"   /* provides build_primes_upto */

static idx_t *g_primes = NULL;    /* array of primes (uint64_t values casted to idx_t) */
static size_t g_prime_cnt = 0;    /* number of primes stored                */
static idx_t  g_prime_limit = 0;  /* largest prime already generated        */

/* Ensure that g_primes contains all primes ≤ limit, allocating or resizing as needed. */
static void
ensure_primes_upto(idx_t limit)
{
    if (limit <= g_prime_limit) return;

    /* free any previously allocated array */
    if (g_primes) free(g_primes);

    size_t cnt = 0;
    uint64_t *tmp = NULL;
    build_primes_upto(limit, &tmp, &cnt);

    g_primes     = (idx_t *)tmp;
    g_prime_cnt  = cnt;
    g_prime_limit = limit;
}

seq_t a(idx_t n)
{
    /* n==0 is the special case */
    if (n == 0U) return (seq_t)1U;

    /* odd indices: 2 if ((n-1)/2) is even, otherwise 1 */
    if (n & 1U) {
        idx_t t = (n - 1U) >> 1U;           /* t = (n-1)/2 */
        return (t & 1U) ? (seq_t)1U : (seq_t)2U;
    }

    /* even indices: product of all primes p with (p-1) | n */
    idx_t m = n;
    /* we need primes up to m+1 because p-1 | m implies p ≤ m+1 */
    ensure_primes_upto(m + 1U);

    seq_t prod = (seq_t)1U;
    for (size_t i = 0; i < g_prime_cnt; ++i) {
        idx_t p = g_primes[i];
        if (p > m + 1U) break;
        if (m % (p - 1U) == 0U) {
            prod = prod * (seq_t)p;
        }
    }
    return prod;
}
