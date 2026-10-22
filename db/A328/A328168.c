/*
 * A328168  Numbers whose prime indices minus 1 are relatively prime.
 * https://oeis.org/A328168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) enumerates integers whose prime indices minus 1 are
 *            relatively prime
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   build_spf, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/build_spf.h"
#include "../utils/gcd.h"

static bool is_valid(idx_t n, idx_t *spf, idx_t *prime_idx)
{
    seq_t g = 0;
    idx_t t = n;
    while (t > 1)
    {
        idx_t p = spf[t];
        idx_t val = prime_idx[p] - 1;        /* p is prime, so prime_idx[p] > 0 */
        if (val != 0)
        {
            seq_t sval = (seq_t)val;
            if (g == 0)
                g = sval;
            else
                g = gcd(g, sval);
        }
        while (t % p == 0)
            t /= p;
    }
    return g == 1;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static idx_t next_candidate = 2;

    static idx_t *spf = NULL;
    static idx_t spf_limit = 0;
    static idx_t *prime_idx = NULL;

    if (n == 0)
        return 0;  /* undefined, but safe fallback */

    while (cache_len < (size_t)n)
    {
        /* Extend SPF and prime index tables if needed */
        if (next_candidate > spf_limit)
        {
            idx_t new_limit = spf_limit == 0 ? 2 : spf_limit * 2;
            while (new_limit < next_candidate)
                new_limit *= 2;

            free(spf);
            free(prime_idx);
            build_spf(new_limit, &spf);
            spf_limit = new_limit;

            prime_idx = (idx_t *)malloc((new_limit + 1) * sizeof(idx_t));
            memset(prime_idx, 0, (new_limit + 1) * sizeof(idx_t));
            idx_t idx = 1;
            for (idx_t i = 2; i <= new_limit; ++i)
                if (spf[i] == i)      /* i is prime */
                    prime_idx[i] = idx++;
        }

        if (is_valid(next_candidate, spf, prime_idx))
        {
            if (cache_len == cache_cap)
            {
                cache_cap = cache_cap == 0 ? 128 : cache_cap * 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
