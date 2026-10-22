/*
 * A321613  Partial products of the unitary totient function (A047994): a(n) =
 *          Product_{k=1..n} uphi(k).
 * https://oeis.org/A321613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=1..n} uphi(k) where uphi(p^e) = p^e – 1 and uphi is
 *            multiplicative over coprime factors. See OEIS A321613
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 15, u64 n <= 23, u128 n <= 38,
 *            big any n
 * Helpers:   big_pow, grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/big_pow.h"   /* seq_t big_pow(seq_t base, seq_t exp) */
#include "../utils/grow_seq.h"  /* void grow_seq(seq_t **arr, size_t *cap, size_t need) */

/* Helper to compute the smallest prime factor array up to a given limit. */
static uint64_t *spfNULL = NULL;        /* array of SPF values, index i has SPF(i) */
static idx_t spf_limit = 0;             /* current computed limit */

/* Ensure the SPF array covers all numbers up to 'limit'. */
static void ensure_spf(idx_t limit)
{
    if (limit <= spf_limit)
        return;           /* already computed */

    /* Allocate a new array with the required size. */
    size_t new_cap = (size_t)limit + 1;        /* include index 0 */
    uint64_t *new_spf = (uint64_t *)malloc(new_cap * sizeof(uint64_t));
    if (!new_spf)
        abort();                                /* allocation failure */

    /* Initialize SPF values to 0 (unsieved). */
    for (size_t i = 0; i < new_cap; ++i)
        new_spf[i] = 0;

    /* Sieve: fill SPF for 2..limit. */
    for (uint64_t i = 2; i <= limit; ++i)
    {
        if (new_spf[i] == 0)        /* i is prime */
        {
            new_spf[i] = i;
            if (i <= limit / i)     /* avoid overflow */
            {
                for (uint64_t j = i * i; j <= limit; j += i)
                    if (new_spf[j] == 0)
                        new_spf[j] = i;
            }
        }
    }

    /* Free old array and update pointers. */
    free(spfNULL);
    spfNULL = new_spf;
    spf_limit = limit;
}

/* Cache for partial products a(n).  The array is indexed by n (1‑based). */
static seq_t *a_cache    = NULL;   /* array of a(n) values */
static size_t a_cap      = 0;       /* allocated size (≥ a_len+1) */
static idx_t   a_len     = 0;       /* highest n for which a_cache[n] is valid */

/* Main function required by the judge. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;        /* undefined for n < 1, but guard */

    /* Ensure SPF up to n. */
    ensure_spf(n);

    /* Ensure cache has room for indices up to n. */
    if (n + 1 > a_cap)
        grow_seq(&a_cache, &a_cap, (size_t)n + 1);

    /* If a_cache already contains requested n, return it. */
    if (n <= a_len)
        return a_cache[n];

    /* Initialise first element if necessary. */
    if (a_len == 0)
    {
        a_cache[1] = (seq_t)1;
        a_len = 1;
    }

    /* Compute missing values from a_len+1 up to n. */
    for (idx_t k = a_len + 1; k <= n; ++k)
    {
        /* Compute uphi(k). */
        seq_t uphi = (seq_t)1;
        uint64_t x   = (uint64_t)k;

        while (x > 1)
        {
            uint64_t p   = spfNULL[x];
            uint64_t exp = 0;
            do
            {
                x /= p;
                ++exp;
            } while (x % p == 0);

            seq_t power = big_pow((seq_t)p, (seq_t)exp);  /* p^exp */
            uphi = uphi * (power - (seq_t)1);            /* multiply by (p^exp - 1) */
        }

        a_cache[k] = a_cache[k - 1] * uphi;   /* cumulative product */
    }

    a_len = n;        /* mark up to n as computed */
    return a_cache[n];
}
