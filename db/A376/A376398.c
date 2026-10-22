/*
 * A376398  a(n) = A276085(n!), where A276085 is the primorial base
 *          log-function.
 * https://oeis.org/A376398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 30, u64 n <= 58, u128 n <= 106,
 *            big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A376398
 *  Formula: a(n) = A276085(n!) = Σ_{i=1..n} A276085(i)
 *  A276085(n) = Σ_{p^e||n} e * Π_{q<p} q   (product of primes < p)
 *
 *  This implementation builds a sieve of smallest prime factors (spf),
 *  precomputes the primorial less values for each prime, and then
 *  accumulates the partial sums of A276085(i).  Caching is used to
 *  avoid recomputing for repeated or decreasing values of n.
 *
 *  All arithmetic is performed using the seq_t type; overflow is
 *  naturally handled by the underlying unsigned integer type.
 *
 *  Indices start at 1 (a(1) = 0).  a(0) returns 0 for safety.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/build_spf.h"

/* Cached tables */
static idx_t *spf_arr      = NULL;   /* spf_arr[i] = smallest prime factor of i */
static seq_t *primorial_less = NULL; /* primorial_less[p] = Π_{q<p} q (0 if p=2) */
static seq_t *partial_sum   = NULL; /* partial_sum[i] = a(i) for i <= max_n */
static idx_t  max_n         = 0;     /* maximum n for which tables are built */

/* ------------------------------------------------------------------ */
/*  Compute A276085 for a single integer n (using cached spf and        */
/*  primorial_less tables).  The result is of type seq_t.              */
/* ------------------------------------------------------------------ */
static seq_t primorial_log(idx_t n)
{
    seq_t total = (seq_t)0;
    idx_t v     = n;

    while (v > 1) {
        idx_t p   = spf_arr[v];
        idx_t exp = 0;
        while (v % p == 0) {
            v /= p;
            ++exp;
        }
        total += (seq_t)exp * primorial_less[p];
    }
    return total;
}

/* ------------------------------------------------------------------ */
/*  Ensure that all cached tables have been built up to at least n.    */
/*  Rebuilds all tables if the requested limit exceeds the current    */
/*  maximum.  The rebuild is inexpensive for n up to at least 10'000. */
/* ------------------------------------------------------------------ */
static void ensure_cache(idx_t n)
{
    if (n <= max_n)
        return;   /* already built */

    /* free previous tables */
    if (spf_arr)      free(spf_arr);
    if (primorial_less) free(primorial_less);
    if (partial_sum)  free(partial_sum);

    /* Build new tables up to limit n */
    build_spf(n, &spf_arr);

    primorial_less = (seq_t*)calloc((size_t)(n + 1), sizeof(seq_t));
    seq_t prod = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i) {
        if (spf_arr[i] == i) {          /* i is prime */
            primorial_less[i] = prod;   /* product of primes < i */
            prod = prod * (seq_t)i;     /* update primorial for next prime */
        }
    }

    partial_sum = (seq_t*)malloc((size_t)(n + 1) * sizeof(seq_t));
    partial_sum[0] = (seq_t)0;
    for (idx_t i = 1; i <= n; ++i) {
        partial_sum[i] = partial_sum[i - 1] + primorial_log(i);
    }

    max_n = n;
}

/* ------------------------------------------------------------------ */
/*  Public function:  seq_t a(idx_t n)                                */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n=0; return 0 for safety */

    ensure_cache(n);
    return partial_sum[n];
}
