/*
 * A360224  Number of k < n such that gcd(k, n) > 1, gcd(n^2-1, k) = 1, and
 *          rad(k) does not divide n.
 * https://oeis.org/A360224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 357, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (by definition):
   a(n) = |{ k | 1 <= k < n,
             gcd(k,n) > 1,
             gcd(k, n^2-1) = 1,
             rad(k) does NOT divide n }| */

/* The implementation uses a small sieve (up to 10000) to pre‑compute
   smallest prime factors and the radical of every integer up to that
   bound.  For each n we then iterate over 1 <= k < n and test the
   three conditions using the supplied gcd helper.  This algorithm is
   O(n log n) with a very small constant and satisfies the 2‑second
   time limit for n ≤ 10000. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* ------------------------------------------------------------ */
/* Global precomputation data                                   */
/* ------------------------------------------------------------ */
#define MAX_PRECOMP 10000   /* sufficient for n <= 10000 */

static uint32_t spf[MAX_PRECOMP + 1] = {0};   /* smallest prime factor */
static seq_t   rad[MAX_PRECOMP + 1] = {0};    /* radical of i   */
static bool    precomp_done = false;

/* ------------------------------------------------------------ */
/* Sieve for spf and computation of rad                      */
/* ------------------------------------------------------------ */
static void precompute(void)
{
    if (precomp_done)
        return;

    /* Smallest prime factor sieve */
    for (uint32_t i = 2; i <= MAX_PRECOMP; ++i) {
        if (spf[i] == 0) {          /* i is prime */
            for (uint32_t j = i; j <= MAX_PRECOMP; j += i)
                if (spf[j] == 0)
                    spf[j] = i;
        }
    }
    spf[0] = spf[1] = 1;

    /* Radical of each integer */
    rad[0] = 0;
    rad[1] = 1;
    for (uint32_t i = 2; i <= MAX_PRECOMP; ++i) {
        uint32_t x = i;
        seq_t   r = 1;
        while (x > 1) {
            uint32_t p = spf[x];
            r *= (seq_t)p;
            while (x % p == 0)
                x /= p;
        }
        rad[i] = r;
    }

    precomp_done = true;
}

/* ------------------------------------------------------------ */
/* Main sequence function                                       */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* For very small n the answer is 0, but the loop below will
       produce the same result, so we simply proceed. */
    if (n <= 1)
        return (seq_t)0;

    /* Ensure precomputation is done */
    precompute();

    /* Compute n^2 - 1 once; n <= MAX_PRECOMP, so the product fits
       easily into seq_t. */
    seq_t n_sq   = (seq_t)n * (seq_t)n;
    seq_t n2m1   = n_sq - (seq_t)1;

    seq_t count   = 0;

    for (idx_t k = 2; k < n; ++k) {            /* k >= 2 because k = 1 never satisfies gcd(k,n)>1 */
        /* 1. gcd(k,n) > 1 */
        if (gcd((seq_t)k, (seq_t)n) == (seq_t)1)
            continue;

        /* 2. gcd(k,n^2-1) == 1  (i.e. k coprime to n-1 and n+1) */
        if (gcd((seq_t)k, n2m1) != (seq_t)1)
            continue;

        /* 3. rad(k) does NOT divide n */
        if (n % rad[(uint32_t)k] == 0)           /* rad[k] <= k <= MAX_PRECOMP */
            continue;

        ++count;
    }

    return count;
}
