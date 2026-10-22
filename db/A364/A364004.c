/*
 * A364004  Orders of simple groups PSL(2,K) with exactly 4 prime divisors.
 * https://oeis.org/A364004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = q*(q^2-1)/gcd(2, q-1) where q is the nth prime power (>1)
 *            * such that the simple group PSL(2,q) has exactly four distinct
 *            prime divisors
 * Range:     u8 none, u16 n <= 12, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   uniq_add
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/uniq_add.h"

/* ------- global cache for the sequence ------- */
static seq_t *cache     = NULL;   /* a(k) values   */
static size_t cache_used = 0;     /* number of terms cached */
static size_t cache_cap  = 0;     /* capacity of cache */

static idx_t next_q = 2;          /* next q to test (prime power) */

/* ------- prime sieve infrastructure ------- */
static int *spf   = NULL;    /* smallest prime factor for each i */
static int *primes = NULL;   /* list of primes up to sieve_lim */
static size_t prime_cnt = 0;
static size_t sieve_lim = 0;

/* build sieve up to 'limit', inclusive */
static void build_sieve(size_t limit)
{
    free(spf);
    free(primes);
    spf     = (int*)calloc(limit + 1, sizeof(int));
    primes  = (int*)malloc((limit + 1) * sizeof(int));
    prime_cnt = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (!spf[i]) {
            spf[i] = (int)i;
            primes[prime_cnt++] = (int)i;
        }
        for (size_t j = 0; j < prime_cnt &&
                        (size_t)primes[j] <= (size_t)spf[i] &&
                        i * (size_t)primes[j] <= limit; ++j) {
            spf[i * (size_t)primes[j]] = primes[j];
        }
    }
    sieve_lim = limit;
}

static void ensure_sieve(idx_t need)
{
    if ((size_t)need <= sieve_lim) return;
    size_t limit = sieve_lim ? sieve_lim * 2u : 1024u;
    while (limit < (size_t)need + 1u) limit <<= 1u;
    build_sieve(limit);
}

/* enlarge the cache as needed */
static void ensure_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2u : 64u;
    while (new_cap < needed) new_cap <<= 1u;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* test whether n (>1) is a prime power */
static bool is_prime_power(idx_t n)
{
    if (n < 2) return false;
    int cnt = 0;
    int prev = 0;
    idx_t tmp = n;
    while (tmp > 1) {
        int p = spf[(size_t)tmp];
        if (p == 0) p = (int)tmp;          /* tmp is prime > sieve limit */
        if (p != prev) {
            cnt++;
            if (cnt > 1) return false;
            prev = p;
        }
        tmp /= p;
    }
    return cnt == 1;
}

/* factor n into distinct primes, storing them in 'arr', update '*cnt' */
static void factor_distinct(idx_t n, int *arr, size_t *cnt)
{
    *cnt = 0;
    int prev = 0;
    while (n > 1) {
        int p = spf[(size_t)n];
        if (p == 0) p = (int)n;
        if (p != prev) {
            arr[(*cnt)++] = p;
            prev = p;
        }
        n /= p;
    }
}

/* main sequence function ------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, but safe guard */

    if (cache_used < (size_t)n) {
        ensure_cache((size_t)n);
        /* generate terms until we reach n */
        while (cache_used < (size_t)n) {
            /* sieve must cover q-1, q, q+1 */
            ensure_sieve(next_q + 1u);

            if (is_prime_power(next_q)) {
                /* collect distinct primes from q, q-1, q+1 */
                int primes_q[16], primes_m[16], primes_p[16];
                size_t cnt_q, cnt_m, cnt_p;
                factor_distinct(next_q,     primes_q,  &cnt_q);
                factor_distinct(next_q-1,   primes_m,  &cnt_m);
                factor_distinct(next_q+1,   primes_p,  &cnt_p);

                int uniq[32];
                size_t uniq_cnt = 0;
                for (size_t i = 0; i < cnt_q; ++i) uniq_add(primes_q[i], uniq, &uniq_cnt, sizeof(uniq)/sizeof(uniq[0]));
                for (size_t i = 0; i < cnt_m; ++i) uniq_add(primes_m[i], uniq, &uniq_cnt, sizeof(uniq)/sizeof(uniq[0]));
                for (size_t i = 0; i < cnt_p; ++i) uniq_add(primes_p[i], uniq, &uniq_cnt, sizeof(uniq)/sizeof(uniq[0]));

                if (uniq_cnt == 4) {
                    seq_t qseq = (seq_t)next_q;
                    seq_t val;
                    if (next_q & 1) {         /* odd q : divide by 2 */
                        seq_t t = qseq * qseq;
                        t -= 1;
                        t /= 2;
                        val = qseq * t;
                    } else {                  /* even q : no division */
                        seq_t t = qseq * (qseq - 1);
                        val = t * (qseq + 1);
                    }
                    cache[cache_used++] = val;
                }
            }
            ++next_q;
        }
    }
    return cache[n - 1];
}
