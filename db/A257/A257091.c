/*
 * A257091  a(n) = log_5 (A256693(n)).
 * https://oeis.org/A257091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = log_5(A256693(n)) = bigomega(n) + Σ_{primes p}
 *            floor(e_p/5) * with bigomega(n) the total number of prime
 *            factors of n counted with multiplicity. * Recurrence: if n = p^e
 *            · m (p is the smallest prime factor of n, e≥1, gcd(p,m)=1) *
 *            a(n) = a(m) + e + floor(e/5) + floor(e/25) + …
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* ---------- cache of computed values ---------- */
static seq_t *cache = NULL;      /* a(n) for n ≥ 1 (index 0 unused) */
static idx_t cache_cap = 0;      /* largest n that has been cached */

/* ---------- smallest‑prime‑factor table ---------- */
static idx_t *spf = NULL;        /* spf[i] = smallest prime factor of i */
static idx_t spf_limit = 0;      /* largest i for which spf is valid */

/* ensure spf table exists up to n (inclusive) */
static void ensure_spf(idx_t n)
{
    if (n <= spf_limit)
        return;

    /* Re‑allocate spf array */
    if (spf != NULL)
        free(spf);
    spf = (idx_t *)malloc((n + 1) * sizeof(idx_t));
    if (!spf)
        abort();

    for (idx_t i = 0; i <= n; ++i)
        spf[i] = i;

    for (idx_t i = 2; i * i <= n; ++i) {
        if (spf[i] == i) {            /* i is prime */
            for (idx_t j = i * i; j <= n; j += i)
                if (spf[j] == j)
                    spf[j] = i;
        }
    }
    spf_limit = n;
}

/* ensure that a(1)…a(n) are computed and stored in cache */
static void ensure_cache(idx_t n)
{
    if (n <= cache_cap)
        return;                       /* already computed */

    /* Build spf table up to n */
    ensure_spf(n);

    /* Allocate or grow cache array */
    if (cache == NULL) {
        cache = (seq_t *)calloc(n + 1, sizeof(seq_t));
        if (!cache)
            abort();
        cache[1] = 0;                  /* a(1) = 0 */
    } else {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp)
            abort();
        cache = tmp;
        /* new memory is uninitialised; we will compute it below */
    }

    /* Compute newly required terms */
    for (idx_t i = cache_cap == 0 ? 2 : cache_cap + 1; i <= n; ++i) {
        idx_t val = i;
        idx_t p   = spf[val];         /* smallest prime factor of i */
        idx_t e   = 0;
        while (val % p == 0) {        /* exponent of p in i */
            val /= p;
            ++e;
        }
        idx_t f   = val;              /* remaining cofactor (i / p^e) */

        /* term = e + floor(e/5) + floor(e/25) + …  */
        seq_t term = e;
        seq_t ee   = e;
        while (ee >= 5) {
            ee /= 5;
            term += ee;
        }
        cache[i] = cache[f] + term;
    }

    cache_cap = n;
}

/* ------------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)                /* undefined, return 0 safely */
        return 0;
    ensure_cache(n);
    return cache[n];
}
