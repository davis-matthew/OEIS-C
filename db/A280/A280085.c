/*
 * A280085  Partial sums of A206032 (Product_{d|n} sigma(d)).
 * https://oeis.org/A280085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..n} A206032(i) * where A206032(i) = ∏_{d|i} σ(d)
 *            * and σ(d) is the sum of the divisors of d
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Smallest prime factor array */
static uint32_t *spf_arr = NULL;
static size_t   spf_max  = 0;

/* Build or extend the spf sieve up to 'limit'. */
static void build_spf(size_t limit)
{
    if (limit <= spf_max)
        return;

    spf_arr = (uint32_t *)realloc(spf_arr, (limit + 1) * sizeof(uint32_t));
    if (!spf_arr)
        exit(1);

    /* initialise the array */
    for (size_t i = 0; i <= limit; ++i)
        spf_arr[i] = 0;

    for (size_t i = 2; i <= limit; ++i) {
        if (spf_arr[i] == 0) {
            spf_arr[i] = (uint32_t)i;
            if (i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i)
                    if (spf_arr[j] == 0)
                        spf_arr[j] = (uint32_t)i;
            }
        }
    }
    spf_max = limit;
}

/* Compute A206032(n) = ∏_{d|n} σ(d) multiplicatively. */
static seq_t compute_A206032(uint64_t n)
{
    if (n == 1)
        return (seq_t)1;          /* σ(1) = 1, product over single divisor */

    uint64_t tmp = n;
    unsigned primes[64];
    int exps[64];
    int pcnt = 0;

    while (tmp > 1) {
        uint32_t p = spf_arr[(size_t)tmp];
        int e = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++e;
        }
        primes[pcnt] = p;
        exps[pcnt]   = e;
        ++pcnt;
    }

    /* number of divisors d(n) = ∏ (e_i + 1) */
    seq_t dcount = 1;
    for (int i = 0; i < pcnt; ++i)
        dcount *= (seq_t)(exps[i] + 1);

    seq_t result = 1;

    for (int i = 0; i < pcnt; ++i) {
        int e = exps[i];
        seq_t g = 1;

        seq_t pow = 1;      /* p^0 */
        seq_t sigma_val = 1; /* σ(p^0) */

        for (int k = 0; k <= e; ++k) {
            if (k == 0) {
                sigma_val = 1;
                pow = 1;
            } else {
                pow *= (seq_t)primes[i];
                sigma_val += pow;   /* σ(p^k) = σ(p^{k-1}) + p^k */
            }
            g *= sigma_val;
        }

        /* exponent = d(n) / (e + 1) */
        uint64_t exp_uint = (uint64_t)(dcount / (seq_t)(e + 1));
        for (uint64_t t = 0; t < exp_uint; ++t)
            result *= g;
    }

    return result;
}

/* Cache for partial sums a(n) */
static seq_t *a_cache   = NULL;   /* array indexed by n; a_cache[0] = 0 */
static size_t a_cache_size = 0;   /* allocated size of a_cache */
static size_t a_cache_max  = 0;   /* largest n for which a_cache[n] is computed */

seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; n=0 is undefined but return 0 safely */
        return (seq_t)0;

    size_t target = (size_t)n;

    /* Grow the a_cache array as needed */
    if (target > a_cache_max) {
        size_t new_size = target + 1;
        if (new_size < 16)
            new_size = 16;

        a_cache = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
        if (!a_cache)
            exit(1);

        /* initialise first element if this is the first allocation */
        if (a_cache_size == 0)
            a_cache[0] = (seq_t)0;

        a_cache_size = new_size;
    }

    /* Ensure spf sieve large enough */
    build_spf(target);

    /* Compute a(n) for all n beyond the previously computed maximum */
    for (size_t i = a_cache_max + 1; i <= target; ++i) {
        seq_t f = compute_A206032(i);
        a_cache[i] = a_cache[i - 1] + f;
    }

    a_cache_max = target;
    return a_cache[target];
}
