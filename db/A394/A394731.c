/*
 * A394731  Numbers k such that k*(k + 1)/2 + d is a triangular number
 *          (A000217), where d > (k + 1) is a divisor of k*(k + 1)/2.
 * https://oeis.org/A394731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th integer k for which k*(k+1)/2 + d is
 *            triangular, where d > (k+1) is a divisor of k*(k+1)/2
 * Range:     u8 none, u16 n <= 2, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation searches for the values lazily and memoises them.
   It factors k*(k+1)/2 into (k or k+1) times ((k+1) or k)/2,
   enumerates all its divisors using a recursive DFS, and checks the
   triangularity criterion via 8x+1 being an odd perfect square. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h"

/* --- Utility: test whether a number is triangular --------------------------------- */
static bool is_triangular(seq_t x)
{
    seq_t d   = x * 8 + 1;
    seq_t s   = floor_sqrt(d);
    bool odd  = (s & 1) == 1;
    bool square = (s * s) == d;
    return odd && square;
}

/* --- Factorizations of 64‑bit integers --------------------------------------------- */
static void factor_uint64(uint64_t n, uint64_t *primes, int *exps, int *cnt)
{
    int c = 0;
    for (uint64_t p = 2; p <= n / p; ++p)
    {
        if (n % p == 0)
        {
            int e = 0;
            while (n % p == 0)
            {
                n /= p;
                ++e;
            }
            primes[c] = p;
            exps[c]   = e;
            ++c;
        }
    }
    if (n > 1)
    {
        primes[c] = n;
        exps[c]   = 1;
        ++c;
    }
    *cnt = c;
}

/* --- Merge two sorted factor lists into a single list ------------------------------ */
static void merge_factors(const uint64_t *a, const int *ea, int na,
                          const uint64_t *b, const int *eb, int nb,
                          uint64_t *out, int *eout, int *nout)
{
    int i = 0, j = 0, k = 0;
    while (i < na && j < nb)
    {
        if (a[i] < b[j])
        {
            out[k]   = a[i];
            eout[k]  = ea[i];
            ++k; ++i;
        }
        else if (a[i] > b[j])
        {
            out[k]   = b[j];
            eout[k]  = eb[j];
            ++k; ++j;
        }
        else
        {
            out[k]   = a[i];
            eout[k]  = ea[i] + eb[j];
            ++k; ++i; ++j;
        }
    }
    while (i < na)
    {
        out[k]   = a[i];
        eout[k]  = ea[i];
        ++k; ++i;
    }
    while (j < nb)
    {
        out[k]   = b[j];
        eout[k]  = eb[j];
        ++k; ++j;
    }
    *nout = k;
}

/* --- Recursive DFS over all divisors of the product given by primes[0..nf-1] --------- */
static bool dfs_divisors(const uint64_t *pr, const int *exp,
                         int nf, int idx, seq_t cur,
                         uint64_t k, seq_t T)
{
    if (idx == nf)
    {
        seq_t limit = (seq_t)(k + 1);
        if (cur > limit)
        {
            seq_t rhs = T + cur;
            return is_triangular(rhs);
        }
        return false;
    }
    seq_t val = cur;
    for (int e = 0; e <= exp[idx]; ++e)
    {
        if (dfs_divisors(pr, exp, nf, idx + 1, val, k, T))
            return true;
        val = val * (seq_t)pr[idx];
    }
    return false;
}

/* --- Test whether a given k satisfies the definition ------------------------------- */
static bool has_property(uint64_t k)
{
    seq_t T = ((seq_t)k) * ((seq_t)k + 1) / 2;

    /* Split k*(k+1)/2 into a*b where a and b are <= k+1 */
    uint64_t a, b;
    if (k & 1)
    {
        a = k;
        b = (k + 1) / 2;
    }
    else
    {
        a = k / 2;
        b = k + 1;
    }

    /* Factor a and b */
    uint64_t primes_a[64], primes_b[64];
    int    exps_a[64],    exps_b[64];
    int na, nb;
    factor_uint64(a, primes_a, exps_a, &na);
    factor_uint64(b, primes_b, exps_b, &nb);

    /* Merge factor lists */
    uint64_t primes[64];
    int    exps[64];
    int nf;
    merge_factors(primes_a, exps_a, na,
                  primes_b, exps_b, nb,
                  primes, exps, &nf);

    return dfs_divisors(primes, exps, nf, 0, (seq_t)1, k, T);
}

/* --- Memoised storage of found sequence values ------------------------------------ */
static seq_t *seq_cache       = NULL;
static size_t seq_cache_sz     = 0;
static size_t seq_cache_cap    = 0;
static uint64_t next_k = 1;      /* next k candidate to test */

/* Ensure that the first n elements of the sequence are computed and cached */
static void ensure_terms(idx_t n)
{
    if ((size_t)n <= seq_cache_sz)
        return;

    size_t need = (size_t)n;
    if (seq_cache_cap < need)
    {
        size_t cap = seq_cache_cap ? seq_cache_cap * 2 : 64;
        while (cap < need)
            cap <<= 1;
        seq_cache = (seq_t *)realloc(seq_cache, cap * sizeof(seq_t));
        seq_cache_cap = cap;
    }

    while (seq_cache_sz < need)
    {
        while (!has_property(next_k))
            ++next_k;
        seq_cache[seq_cache_sz++] = (seq_t)next_k;
        ++next_k;                  /* search for the next candidate after the found one */
    }
}

/* --- Public function -------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)      /* behaviour for n < 1 is undefined */
        return 0;
    ensure_terms(n);
    return seq_cache[n - 1];
}
