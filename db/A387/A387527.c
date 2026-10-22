/*
 * A387527  a(n) = largest k such that, starting at n, the k pairs (n,n+1),
 *          (n+1,n+2), ... (n+k-1,n+k) have the "Distinct Prime Exponents"
 *          (DPE) property.
 * https://oeis.org/A387527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9874, u16 n <= 9874, u32 n <= 9874, u64 n <= 9874,
 *            u128 n <= 9874, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A387527: a(n) = largest k such that the k consecutive pairs
   (n,n+1),(n+1,n+2),…,(n+k-1,n+k) all have the DPE property.
   The implementation checks the DPE property by factoring each
   number using a table of smallest prime factors (SPF). */
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

/* Smallest prime factor cache */
static idx_t *spf = NULL;      /* SPF[0..size] */
static idx_t spf_size = 0;     /* current size of SPF array */

/* Ensure that spf contains the least prime factor for every integer
   up to and including limit.  The array is grown if necessary.  */
static void ensure_spf(idx_t limit)
{
    if (spf != NULL && spf_size >= limit)
        return;                      /* already large enough */

    idx_t new_size = limit + 1;
    idx_t *new_spf = (idx_t *)malloc(new_size * sizeof(idx_t));
    if (!new_spf)
        return;                      /* out of memory: give up */

    /* initialise with zero */
    for (idx_t i = 0; i < new_size; ++i)
        new_spf[i] = 0;

    /* sieve for least prime factor */
    for (idx_t i = 2; i <= limit; ++i) {
        if (new_spf[i] == 0) {                /* i is prime */
            new_spf[i] = i;
            if (i <= limit / i) {             /* start marking at i*i */
                for (idx_t j = i * i; j <= limit; j += i)
                    if (new_spf[j] == 0)
                        new_spf[j] = i;
            }
        }
    }

    free(spf);
    spf = new_spf;
    spf_size = limit;
}

/* Factor m using the SPF table; store the exponents in the array exps,
   and write the count of exponents to *len.  The array must be large
   enough for the number of distinct prime factors of m. */
static void factor_exponents(uint64_t m, uint8_t *exps, size_t *len)
{
    size_t l = 0;
    while (m > 1) {
        uint64_t p = spf[m];
        uint8_t cnt = 0;
        while (m % p == 0) {
            m /= p;
            ++cnt;
        }
        exps[l++] = cnt;
    }
    *len = l;
}

/* Return true iff the exponent sets of a and b are disjoint. */
static bool dpe(uint64_t a, uint64_t b)
{
    idx_t need = a > b ? a : b;
    if (spf_size < need)
        ensure_spf(need);

    static uint8_t exp_a[64], exp_b[64];
    static unsigned char used[64];
    size_t la, lb;

    factor_exponents(a, exp_a, &la);
    factor_exponents(b, exp_b, &lb);

    /* clear used array */
    memset(used, 0, sizeof(used));

    for (size_t i = 0; i < la; ++i)
        used[exp_a[i]] = 1;
    for (size_t i = 0; i < lb; ++i)
        if (used[exp_b[i]])
            return false;
    return true;
}

/* Sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                 /* offset is 1 */

    seq_t k = 0;
    for (idx_t i = 0;; ++i) {
        uint64_t x = (uint64_t)n + i;
        uint64_t y = x + 1;
        if (!dpe(x, y))
            break;
        ++k;
    }
    return k;
}
