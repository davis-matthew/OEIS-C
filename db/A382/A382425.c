/*
 * A382425  The number of non-unitary prime divisors of the biquadratefree
 *          numbers.
 * https://oeis.org/A382425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of primes whose exponent is 2 or 3 in the *
 *            factorisation of the n‑th biquadratefree number. * A
 *            biquadratefree number is a positive integer whose all prime *
 *            exponents are ≤3. The first biquadratefree number is 1, so *
 *            a(1)=0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* --------------------------------------------------------------------- */
/*  cached values of the sequence                                     */
static seq_t *seq_cache = NULL;   /* a(1)..a(cache_len)          */
static size_t cache_len  = 0;     /* values already computed     */
static size_t cache_cap  = 0;     /* buffer capacity              */

/* --------------------------------------------------------------------- */
/*  smallest prime factor table                                        */
static idx_t *spf          = NULL; /* spf[0..spf_limit]            */
static idx_t  spf_limit    = 0;    /* largest index in spf         */

/* --------------------------------------------------------------------- */
/*  generators for biquadratefree numbers                              */
static idx_t gen_next_i  = 1; /* next integer to test          */
static idx_t gen_done    = 0; /* number of biquadratefree numbers produced */

/* --------------------------------------------------------------------- */
/*  build a new spf table up to at least 'limit'                        */
static void rebuild_spf(idx_t limit)
{
    idx_t i, j;
    idx_t *new_spf = (idx_t *)malloc((size_t)(limit + 1) * sizeof(idx_t));
    if (!new_spf) return;                     /* abort on OOM */

    for (i = 0; i <= limit; ++i) new_spf[i] = 0;
    new_spf[0] = 0;
    new_spf[1] = 1;

    for (i = 2; i <= limit; ++i) {
        if (new_spf[i] == 0) {                /* i is prime */
            new_spf[i] = i;
            if ((idx_t)i * (idx_t)i <= limit) {
                for (j = i * i; j <= limit; j += i)
                    if (new_spf[j] == 0) new_spf[j] = i;
            }
        }
    }

    free(spf);
    spf        = new_spf;
    spf_limit  = limit;
}

/* --------------------------------------------------------------------- */
/*  ensure that the cache contains a(1)..a(n)                           */
static void ensure_cache(idx_t n)
{
    if (n <= cache_len) return;               /* already done */

    /* initialise spf table lazily */
    if (spf == NULL) rebuild_spf(64);

    /* grow cache capacity if needed */
    if (cache_cap == 0) cache_cap = 16;
    while (cache_cap < n) cache_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq_cache, cache_cap * sizeof(seq_t));
    if (!tmp) return;                         /* abort on OOM */
    seq_cache = tmp;

    /* generate biquadratefree numbers until we have n of them */
    while (gen_done < n) {
        idx_t cur = gen_next_i;

        /* enlarge spf table if cur is outside its range */
        if (cur > spf_limit) {
            idx_t new_limit = cur * 2 + 100;   /* safety margin */
            rebuild_spf(new_limit);
        }

        /* factorise cur using spf, detect if it is biquadratefree and
         * count primes with exponent ≥2.                     */
        idx_t tmp = cur;
        int   ok = 1;          /* 1 = biquadratefree, 0 = not       */
        int   nonunitary = 0;   /* count of primes with exp 2 or 3   */

        while (tmp > 1 && ok) {
            idx_t p = spf[tmp];
            int   exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                exp++;
            }
            if (exp > 3) { ok = 0; break; }     /* not biquadratefree */
            if (exp >= 2) nonunitary++;
        }

        if (ok) {
            seq_cache[gen_done++] = (seq_t)nonunitary;
        }

        ++gen_next_i;
    }
    cache_len = gen_done;
}

/* --------------------------------------------------------------------- */
/*  required exported function                                          */
seq_t a(idx_t n)
{
    if (n == 0) return 0;       /* undefined, stay safe */
    ensure_cache(n);
    return seq_cache[n - 1];
}
