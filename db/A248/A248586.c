/*
 * A248586  a(n) = Sum_{i=0..n} C(n,i)*C(2i,i)^2.
 * https://oeis.org/A248586
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=0..n} binom(n,i)*binom(2i,i)^2 Results from a
 *            linear recurrence: n^2*a(n) + (-19*n^2+19*n-5)*a(n-1) +
 *            35*(n-1)^2*a(n-2) - 17*(n-1)*(n-2)*a(n-3) = 0 Initial values:
 *            a(0)=1, a(1)=5, a(2)=45
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/*------------------------------------*/
/*    internal cache helpers           */
/*------------------------------------*/
static seq_t *seq_cache = NULL;
static size_t cache_cap = 0;
static idx_t  cache_last = 0;      /* last index that has been computed   */
static int    cache_initialized = 0;

/* make sure storage for indices [0..n] is available       */
static void ensure_cache_size(idx_t n)
{
    if (cache_cap > (size_t)n)
        return;
    size_t new_cap = cache_cap ? cache_cap : 4;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cache = tmp;
    cache_cap = new_cap;
}

/* compute a single new value via the recurrence            */
static seq_t compute_next(idx_t k)
{
    /* k >= 3 when this is called                                   */
    if (sizeof(seq_t) <= 8) {
        /* use 128‑bit arithmetic to avoid overflow of intermediate values */
        __int128 N   = (__int128)k;
        __int128 Nm1 = N - 1;
        __int128 Nm2 = N - 2;
        __int128 term1 = ( ((__int128)19 * N * N - (__int128)19 * N + 5)
                           * (__int128)seq_cache[k-1] );
        __int128 term2 = ( (__int128)35 * Nm1 * Nm1 )
                           * (__int128)seq_cache[k-2];
        __int128 term3 = ( (__int128)17 * Nm1 * Nm2 )
                           * (__int128)seq_cache[k-3];
        __int128 numer = term1 - term2 + term3;
        __int128 denom = N * N;
        return (seq_t)(numer / denom);
    } else {
        /* rely on seq_t's (arbitrary‑precision) operators              */
        seq_t N   = (seq_t)k;
        seq_t Nm1 = N - 1;
        seq_t Nm2 = N - 2;
        seq_t part1 = (seq_t)19 * N * N - (seq_t)19 * N + (seq_t)5;
        seq_t term1 = part1 * seq_cache[k-1];
        seq_t term2 = (seq_t)35 * Nm1 * Nm1 * seq_cache[k-2];
        seq_t term3 = (seq_t)17 * Nm1 * Nm2 * seq_cache[k-3];
        seq_t numer = term1 - term2 + term3;
        seq_t denom = N * N;
        return numer / denom;
    }
}

/*---------------------------------------------------------------*/
/*  public function                                            */
/*---------------------------------------------------------------*/
seq_t a(idx_t n)
{
    ensure_cache_size(n);

    /* first time: initialise a(0) */
    if (!cache_initialized) {
        seq_cache[0] = (seq_t)1;
        cache_initialized = 1;
        cache_last = 0;
    }

    /* initialise a(1) and a(2) if requested */
    if (n >= 1 && cache_last < 1) {
        seq_cache[1] = (seq_t)5;
        cache_last = 1;
    }
    if (n >= 2 && cache_last < 2) {
        seq_cache[2] = (seq_t)45;
        cache_last = 2;
    }

    /* compute any missing terms up to n */
    for (idx_t k = cache_last + 1; k <= n; ++k) {
        if (k == 0) {
            seq_cache[0] = (seq_t)1;
        } else if (k == 1) {
            seq_cache[1] = (seq_t)5;
        } else if (k == 2) {
            seq_cache[2] = (seq_t)45;
        } else {
            seq_cache[k] = compute_next(k);
        }
    }
    cache_last = n;

    return seq_cache[n];
}
