/*
 * A358777  Dirichlet inverse of A353557, the characteristic function of odd
 *          numbers with an even number of prime factors (counted with
 *          multiplicity).
 * https://oeis.org/A358777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(1) = 1, and for n>1 a(n) = - Σ_{d|n, d<n} A353557(n/d) * a(d),
 *            where A353557(m) = 1 if m is odd and ω(m) (prime factor count
 *            with multiplicity) is even, otherwise 0
 * Range:     i8 n <= 9971, i16 n <= 9971, i32 n <= 9971, i64 n <= 9971,
 *            i128 n <= 9971, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static cache for the sequence values */
static seq_t *a_cache      = NULL;   /* holds a(i+1) */
static size_t a_cache_cap  = 0;
static size_t a_cache_len  = 0;

/* smallest prime factor table */
static uint32_t *spf        = NULL;
static idx_t     spf_limit  = 0;

/*--- Ensure SPF table up to 'limit' -------------------------------------*/
static void ensure_spf(idx_t limit)
{
    if (spf_limit >= limit) return;

    size_t sz = (size_t)limit + 1;   /* indices 0..limit */
    uint32_t *new_spf = (uint32_t *)malloc(sz * sizeof(uint32_t));
    if (!new_spf) exit(EXIT_FAILURE);

    for (size_t i = 0; i < sz; ++i) new_spf[i] = 0;

    for (size_t i = 2; i < sz; ++i) {
        if (new_spf[i] == 0) {          /* i is prime */
            new_spf[i] = (uint32_t)i;
            if (i <= sz / i) {          /* avoid overflow in j = i*i */
                for (size_t j = i * i; j < sz; j += i)
                    if (new_spf[j] == 0)
                        new_spf[j] = (uint32_t)i;
            }
        }
    }

    free(spf);
    spf = new_spf;
    spf_limit = limit;
}

/*--- Compute ω(n) using the SPF table -------------------------------------*/
static inline seq_t bigomega_value(idx_t n)
{
    seq_t count = 0;
    while (n > 1) {
        uint32_t p = spf[(size_t)n];
        n /= p;
        ++count;
    }
    return count;
}

/*--- Ensure a_cache has room for at least 'need' entries -------------------*/
static void ensure_cache(idx_t need)
{
    if (a_cache_cap >= (size_t)need) return;

    size_t new_cap = a_cache_cap ? a_cache_cap : 1;
    while (new_cap < (size_t)need) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    a_cache = tmp;
    a_cache_cap = new_cap;
}

/*--- Public function -------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for n=0, but safe */

    ensure_cache(n);
    ensure_spf(n);

    while (a_cache_len < (size_t)n) {
        ++a_cache_len;                   /* m = a_cache_len is 1‑based index */
        idx_t m = a_cache_len;

        if (m == 1) {
            a_cache[0] = 1;
            continue;
        }

        seq_t sum = 0;
        /* enumerate proper divisors d of m */
        for (idx_t d = 1; d <= m / d; ++d) {
            if (m % d != 0) continue;

            idx_t d1 = d;
            idx_t d2 = m / d;

            if (d1 < m) {
                idx_t t = d2;               /* co‑factor m/d1 */
                seq_t f = (((t & 1ULL) != 0ULL) && ((bigomega_value(t) & 1) == 0))
                          ? 1 : 0;
                sum += a_cache[d1 - 1] * f;
            }

            if (d2 != d1 && d2 < m) {
                idx_t t = d1;               /* co‑factor m/d2 */
                seq_t f = (((t & 1ULL) != 0ULL) && ((bigomega_value(t) & 1) == 0))
                          ? 1 : 0;
                sum += a_cache[d2 - 1] * f;
            }
        }

        a_cache[m - 1] = -sum;
    }

    return a_cache[n - 1];
}
