/*
 * A324928  Matula-Goebel numbers of rooted trees of depth 3.
 * https://oeis.org/A324928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th number whose Matula-Goebel depth is 3
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Maximum number up to which we pre‑compute data.  This is far above the
   values needed for n ≤ 10000, and guarantees enough terms.   */
static const idx_t MAX_LIMIT = 500000;

static idx_t *spf          = NULL;  /* smallest prime factor   */
static idx_t *prime_index  = NULL;  /* prime → 1‑based index   */
static seq_t *depth_cache  = NULL;  /* cached depth of n       */
static idx_t  primes_cnt   = 0;
static idx_t *primes       = NULL;
static int    initialized  = 0;

/* Build sieve for smallest prime factors and list of primes.  */
static void init_data(void)
{
    spf          = (idx_t*)malloc((MAX_LIMIT + 1) * sizeof(idx_t));
    prime_index  = (idx_t*)calloc(MAX_LIMIT + 1, sizeof(idx_t));
    depth_cache  = (seq_t*)malloc((MAX_LIMIT + 1) * sizeof(seq_t));

    /* smallest prime factor sieve */
    for (idx_t i = 0; i <= MAX_LIMIT; ++i) spf[i] = 0;
    for (idx_t i = 2; i <= MAX_LIMIT; ++i) {
        if (spf[i] == 0) {                  /* i is prime          */
            spf[i] = i;
            if (i <= MAX_LIMIT / i) {
                for (idx_t j = i * i; j <= MAX_LIMIT; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }

    /* collect primes and fill their 1‑based index */
    primes     = (idx_t*)malloc((MAX_LIMIT / 2) * sizeof(idx_t)); /* rough bound */
    primes_cnt = 0;
    for (idx_t i = 2; i <= MAX_LIMIT; ++i)
        if (spf[i] == i) {
            prime_index[i] = primes_cnt + 1; /* indices start at 1   */
            primes[primes_cnt++] = i;
        }

    /* initialise depth cache: -1 means not computed  */
    for (idx_t i = 0; i <= MAX_LIMIT; ++i)
        depth_cache[i] = (seq_t)(-1);
    depth_cache[1] = 0;
}

/* Recursive depth computation with memoisation.  */
static seq_t depth_of(idx_t n)
{
    if (n == 1) return 0;
    seq_t v = depth_cache[n];
    if (v != (seq_t)(-1)) return v;

    idx_t m = n;
    seq_t max_sub = 0;
    while (m > 1) {
        idx_t p   = spf[m];          /* prime factor          */
        idx_t idx = prime_index[p];  /* its position in prime list */
        seq_t d   = depth_of(idx);
        if (d > max_sub) max_sub = d;
        while (m % p == 0) m /= p;   /* skip repeated factor   */
    }
    depth_cache[n] = max_sub + 1;
    return max_sub + 1;
}

seq_t a(idx_t n)
{
    if (!initialized) {
        init_data();
        initialized = 1;
    }

    idx_t count = 0;
    for (idx_t num = 2; num <= MAX_LIMIT; ++num) {
        if (depth_of(num) == 3) {
            ++count;
            if (count == n) return (seq_t)num;
        }
    }

    /* The required index is beyond MAX_LIMIT – extend the search.  */
    /* For n ≤ 10000 this situation never occurs.                 */
    for (idx_t num = MAX_LIMIT + 1;; ++num) {
        /* Factorisation on the fly for numbers larger than pre‑computed limit. */
        idx_t m = num, mm = num;
        seq_t max_sub = 0;
        while (m > 1) {
            /* find smallest prime factor by trial division (rarely used) */
            idx_t p = 0;
            for (idx_t d = 2; d * d <= m; ++d) {
                if (m % d == 0) { p = d; break; }
            }
            if (!p) p = m;          /* m is prime */
            /* obtain index of prime p; for very large primes we approximate → invalid */
            idx_t idx = 0;
            /* If prime > MAX_LIMIT, we cannot determine depth; skip this num */
            if (p <= MAX_LIMIT) idx = prime_index[p];
            if (!idx) continue;
            seq_t d = depth_of(idx);
            if (d > max_sub) max_sub = d;
            while (m % p == 0) m /= p;
        }
        seq_t d = max_sub + 1;
        if (d == 3) {
            ++count;
            if (count == n) return (seq_t)num;
        }
    }
}
