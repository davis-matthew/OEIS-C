/*
 * A276537  Alternating binomial sums of the cubes of the central binomial
 *          coefficients.
 * https://oeis.org/A276537
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (-1)^(n-k)*binomial(n,k)*binomial(2*k,k)^3
 *            Implemented via the 4‑term recurrence from OEIS A276537 for
 *            speed and correctness. The first four values are seeded: a(0)=1,
 *            a(1)=7, a(2)=201, a(3)=7375. Recurrence for n ≥ 0:
 *            (n^3+12n^2+48n+64)*a(n+4) -(60n^3+630n^2+2204n+2569)*a(n+3...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache    = NULL;   /* cache of a(0..n) */
static size_t cache_cap = 0;     /* allocated capacity */
static size_t cache_len = 0;     /* number of cached terms */

/* Ensure the cache can store at least 'needed' terms. */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Initialise the cache with the four seed values. */
static void initialize(void)
{
    if (cache_len == 0) {
        ensure_capacity(4);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)7;
        cache[2] = (seq_t)201;
        cache[3] = (seq_t)7375;
        cache_len = 4;
    }
}

seq_t a(idx_t n)
{
    initialize();

    /* Compute any missing terms up to n. */
    if ((size_t)n >= cache_len) {
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i < 4)         /* already present after init */
                continue;

            idx_t k = i - 4;   /* parameter for the recurrence */
            seq_t value = 0;

            if (sizeof(seq_t) == 8) {
                /* 64‑bit seq_t: use 128‑bit intermediates. */
                unsigned __int128 k64 = (unsigned __int128)k;
                unsigned __int128 k2  = k64 * k64;
                unsigned __int128 k3  = k2  * k64;

                unsigned __int128 denom =
                    k3 + 12 * k2 + 48 * k64 + 64;

                unsigned __int128 coeff3 = 60 * k3 + 630 * k2 + 2204 * k64 + 2569;
                unsigned __int128 coeff2 = 186 * k3 + 1674 * k2 + 5037 * k64 + 5067;
                unsigned __int128 coeff1 = 94  * (2 * k3 + 15 * k2 + 37 * k64 + 30);
                unsigned __int128 coeff0 = 63  * (k3 + 6 * k2 + 11 * k64 + 6);

                unsigned __int128 num =
                    coeff3 * (unsigned __int128)(unsigned long long)cache[(size_t)(k + 3)] +
                    coeff2 * (unsigned __int128)(unsigned long long)cache[(size_t)(k + 2)] +
                    coeff1 * (unsigned __int128)(unsigned long long)cache[(size_t)(k + 1)] +
                    coeff0 * (unsigned __int128)(unsigned long long)cache[(size_t) k];

                value = (seq_t)(num / denom); /* quotient fits in 64 bits */
            } else {
                /* seq_t is big‑integer or >64‑bit: use native operations. */
                seq_t kst = (seq_t)k;
                seq_t k2  = kst * kst;
                seq_t k3  = k2 * kst;

                seq_t denom =
                    k3 + (seq_t)12 * k2 + (seq_t)48 * kst + (seq_t)64;

                seq_t coeff3 =
                    (seq_t)60 * k3 + (seq_t)630 * k2 + (seq_t)2204 * kst + (seq_t)2569;
                seq_t coeff2 =
                    (seq_t)186 * k3 + (seq_t)1674 * k2 + (seq_t)5037 * kst + (seq_t)5067;
                seq_t coeff1 =
                    (seq_t)94  * ((seq_t)2 * k3 + (seq_t)15 * k2 + (seq_t)37 * kst + (seq_t)30);
                seq_t coeff0 =
                    (seq_t)63  * (k3 + (seq_t)6 * k2 + (seq_t)11 * kst + (seq_t)6);

                seq_t num =
                    coeff3 * cache[(size_t)(k + 3)] +
                    coeff2 * cache[(size_t)(k + 2)] +
                    coeff1 * cache[(size_t)(k + 1)] +
                    coeff0 * cache[(size_t) k];

                value = num / denom;
            }

            ensure_capacity((size_t)i + 1);
            cache[(size_t)i] = value;
        }
        cache_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
