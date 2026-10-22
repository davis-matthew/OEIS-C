/*
 * A378767  Numbers k that are not prime powers which are divisible by a cube
 *          greater than 1.
 * https://oeis.org/A378767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Function implementing OEIS A378767
   Numbers k that are not prime powers and are divisible by a cube > 1.
   Condition: omega(k) > 1 && max_exponent(k) >= 3.
   The implementation builds the sequence up to the requested index by
   sieving smallest prime factors up to a dynamic limit until at least
   n terms are found.  The sequence is stored in a static cache so that
   subsequent calls are fast. */
#include <stdint.h>
#include <stdlib.h>

static seq_t *seq_cache = NULL;   /* container for the sequence values */
static size_t cache_cap = 0;      /* allocated capacity of seq_cache   */
static size_t cache_len = 0;      /* number of terms currently stored */
static idx_t  current_limit = 0;  /* sieve limit that produced cache_len */

/* ensure seq_cache can store 'needed' elements */
static void grow_cache(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t newcap = cache_cap ? cache_cap * 2 : 16;
    while (newcap < needed)
        newcap *= 2;
    seq_cache = (seq_t *)realloc(seq_cache, newcap * sizeof(seq_t));
    cache_cap = newcap;
}

/* build the sequence up to at least n terms */
static void build_sequence(idx_t n)
{
    if (cache_len >= (size_t)n)
        return;

    /* start with a reasonable estimate, grow while necessary */
    idx_t limit = current_limit ? current_limit : 200;
    while (1) {
        /* allocate array of smallest prime factors (spf) */
        uint32_t *spf = (uint32_t *)calloc((size_t)limit + 1, sizeof(uint32_t));
        if (!spf)
            exit(EXIT_FAILURE);

        /* sieve of Eratosthenes for spf */
        for (idx_t i = 2; i <= limit; ++i) {
            if (!spf[i]) {
                spf[i] = (uint32_t)i;
                if (i <= limit / i) {
                    for (idx_t j = i * i; j <= limit; j += i)
                        if (!spf[j])
                            spf[j] = (uint32_t)i;
                }
            }
        }

        /* reset cache for new sweep */
        cache_len = 0;
        grow_cache(limit / 10 + 16);

        /* enumerate numbers up to 'limit' and collect qualifying terms */
        for (idx_t x = 2; x <= limit; ++x) {
            idx_t v = x;
            idx_t distinct = 0;
            idx_t max_exp = 0;

            while (v > 1) {
                idx_t p = (idx_t)spf[v];
                idx_t exp = 0;
                while (v % p == 0) {
                    v /= p;
                    exp++;
                }
                distinct++;
                if (exp > max_exp)
                    max_exp = exp;
            }

            if (distinct > 1 && max_exp >= 3) {
                grow_cache(cache_len + 1);
                seq_cache[cache_len++] = (seq_t)x;
                if (cache_len >= (size_t)n) {
                    free(spf);
                    current_limit = limit;
                    return;
                }
            }
        }

        free(spf);
        /* not enough terms yet – increase the limit and try again */
        limit *= 2;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;               /* undefined for n < 1; guard anyway */
    build_sequence(n);
    return seq_cache[n - 1];
}
