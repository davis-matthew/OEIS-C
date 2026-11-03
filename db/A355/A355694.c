/*
 * A355694  Dirichlet inverse of A277791, denominator of sum of reciprocals of
 *          proper divisors of n.
 * https://oeis.org/A355694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 12, i16 n <= 1753, i32 n <= 12000, i64 n <= 12000,
 *            i128 n <= 12000, big any n
 * Validated: standard, pass, 91 terms through n = 9967 (2026-11-03)
 * Helpers:   factor_u64, gcd, ipow_u64_idx, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1; for n > 1,
   a(n) = - Σ_{d|n, d<n}  b(n/d) * a(d)   ,  where   b(m) = m / gcd(m,σ(m)-1),
   and σ(m) = Σ_{d|m} d   (sum of all divisors of m). */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"
#include "../utils/ipow_u64_idx.h"
#include "../utils/mem.h"

/* ----------  ensure arrays can store index n  ---------- */
static void
ensure_cached(size_t n, seq_t **arr, bool **done, size_t *cap)
{
    if (n < *cap)
        return;
    size_t newcap = (*cap == 0) ? 1 : *cap * 2;
    while (newcap <= n)
        newcap <<= 1;                      /* grow to cover n */
    seq_t *new_arr = mem_resize(seq_t, *arr, newcap);
    if (!new_arr)
        exit(1);
    bool *new_done = (bool *)realloc(*done, newcap * sizeof(bool));
    if (!new_done)
        exit(1);
    /* initialise new slots */
    for (size_t i = *cap; i < newcap; ++i)
        new_done[i] = false;
    *arr = new_arr;
    *done = new_done;
    *cap = newcap;
}

/* ----------  cache for a(n) ---------- */
static seq_t *a_cache = NULL;
static bool   *a_done  = NULL;
static size_t  a_cap   = 0;

/* ----------  compute σ(n) (sum of divisors) ---------- */
static seq_t
sigma_uint(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    uint64_t primes[32];
    uint64_t exps[32];
    size_t pcnt = 0;
    factor_u64((uint64_t)n, primes, exps, &pcnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p   = primes[i];
        uint64_t e   = exps[i];
        seq_t   term = ipow_u64_idx(p, e + 1u) - (seq_t)1;  /* p^{e+1} - 1 */
        seq_t   denom = (seq_t)p - (seq_t)1;            /* p - 1 */
        result = (result * term) / denom;              /* exact division */
    }
    return result;
}

/* ----------  compute b(n) = denominator of sum_{d|n,d<n} 1/d ---------- */
static seq_t
compute_b(idx_t n)
{
    seq_t sigma = sigma_uint(n);
    seq_t sigma_minus_one = sigma - (seq_t)1;
    seq_t n_seq = (seq_t)n;
    seq_t g = gcd(n_seq, sigma_minus_one);   /* gcd is always positive */
    return n_seq / g;
}

/* ----------  main sequence function ---------- */
seq_t
a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                     /* undefined for n<1 */

    if (n > (idx_t)SIZE_MAX)                 /* safety guard */
        return (seq_t)0;

    size_t sn = (size_t)n;                   /* index for arrays */

    ensure_cached(sn, &a_cache, &a_done, &a_cap);

    if (a_done[sn])
        return a_cache[sn];

    /* base case */
    if (n == 1) {
        a_cache[sn] = (seq_t)1;
        a_done[sn] = true;
        return a_cache[sn];
    }

    seq_t sum = (seq_t)0;

    /* iterate over proper divisors */
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d != 0)
            continue;

        idx_t q = n / d;                     /* complementary divisor */

        /* divisor d (always < n for n>1) */
        seq_t ad = a(d);                     /* memoised recursion   */
        seq_t bk = compute_b(q);              /* b(n/d)                 */
        sum += ad * bk;

        /* complementary divisor q, if distinct and proper */
        if (q != d && q != n) {
            seq_t aq = a(q);                 /* memoised recursion   */
            seq_t bd = compute_b(d);          /* b(n/q) = b(d)        */
            sum += aq * bd;
        }
    }

    a_cache[sn]  = -sum;
    a_done[sn]   = true;
    return a_cache[sn];
}
