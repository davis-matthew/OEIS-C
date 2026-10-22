/*
 * A344051  a(n) = Sum_{k=0..n} binomial(n, k)*|Lah(n, k)|. Binomial
 *          convolution of the unsigned Lah numbers A271703.
 * https://oeis.org/A344051
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} binomial(n,k)·|Lah(n,k)| Uses the D‑finite
 *            recurrence from OEIS (valid for n≥5)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static seq_t *cache = NULL;
static idx_t  cache_len = 0;

/* External linkage, as required. */
seq_t a(idx_t n)
{
    /* Return cached value if already computed. */
    if (n < cache_len)
        return cache[n];

    /* Ensure capacity of the cache. */
    idx_t needed = n + 1;
    if (cache == NULL) {
        cache = (seq_t *)malloc((needed > 5 ? needed : 5) * sizeof(seq_t));
        cache_len = 0;
    }
    if (needed > cache_len) {
        cache = (seq_t *)realloc(cache, (needed > 5 ? needed : 5) * sizeof(seq_t));
        cache_len = 0;  /* capacity may have changed; rebuild length later */
    }

    /* Initialize base terms if the cache has not been filled yet. */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)37;
        cache[4] = (seq_t)361;
        cache_len = 5;
    }

    /* Compute terms up to n using the recurrence. */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i < 5)
            continue;  /* already set */

        seq_t A1 = cache[i-1];
        seq_t A2 = cache[i-2];
        seq_t A3 = cache[i-3];
        seq_t A4 = cache[i-4];
        seq_t A5 = cache[i-5];

        /* Polynomial coefficients for the recurrence. */
        long long c1 = 6LL * (-8LL * i * i + 5LL * i - 1LL);
        long long c2 = 48LL * i * i * i - 266LL * i * i + 407LL * i - 167LL;
        long long c3 = -16LL * i * i * i * i + 106LL * i * i * i
                     - 219LL * i * i + 108LL * i + 93LL;
        long long c4 = (i - 4) * (2LL * i * i * i - 13LL * i * i + 16LL * i + 25LL);
        long long c5 = -(i - 5) * (i - 4) * (i - 4) * (i - 4);

        if (sizeof(seq_t) <= 8) {
            /* Use 128‑bit arithmetic to avoid overflow for 64‑bit builds. */
            unsigned long long ua1 = (unsigned long long)A1;
            unsigned long long ua2 = (unsigned long long)A2;
            unsigned long long ua3 = (unsigned long long)A3;
            unsigned long long ua4 = (unsigned long long)A4;
            unsigned long long ua5 = (unsigned long long)A5;

            __int128 numer = 0;
            if (c1 >= 0) numer -= (__int128)c1 * ua1;
            else        numer += (__int128)(-c1) * ua1;
            if (c2 >= 0) numer -= (__int128)c2 * ua2;
            else        numer += (__int128)(-c2) * ua2;
            if (c3 >= 0) numer -= (__int128)c3 * ua3;
            else        numer += (__int128)(-c3) * ua3;
            if (c4 >= 0) numer -= (__int128)c4 * ua4;
            else        numer += (__int128)(-c4) * ua4;
            if (c5 >= 0) numer -= (__int128)c5 * ua5;
            else        numer += (__int128)(-c5) * ua5;

            unsigned long long denom = 16ULL * (unsigned long long)i;
            seq_t result = (seq_t)(numer / denom);
            cache[i] = result;
        } else {
            /* Perform all arithmetic with the full seq_t type. */
            seq_t numer = (seq_t)0;
            if (c1 >= 0) numer -= (seq_t)c1 * A1;
            else        numer += (seq_t)(-c1) * A1;
            if (c2 >= 0) numer -= (seq_t)c2 * A2;
            else        numer += (seq_t)(-c2) * A2;
            if (c3 >= 0) numer -= (seq_t)c3 * A3;
            else        numer += (seq_t)(-c3) * A3;
            if (c4 >= 0) numer -= (seq_t)c4 * A4;
            else        numer += (seq_t)(-c4) * A4;
            if (c5 >= 0) numer -= (seq_t)c5 * A5;
            else        numer += (seq_t)(-c5) * A5;

            seq_t denom = (seq_t)(16ULL * (unsigned long long)i);
            seq_t result = numer / denom;
            cache[i] = result;
        }
    }

    cache_len = n + 1;
    return cache[n];
}
