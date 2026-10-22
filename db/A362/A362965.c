/*
 * A362965  Number of primes <= the n-th prime power.
 * https://oeis.org/A362965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes <= the n-th prime power
 * Range:     u8 n <= 283, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* forward declarations of static helpers */
static void recompute(idx_t limit);
static void ensure_cache(idx_t n);

/* static state */
static bool initialized = false;
static idx_t sieve_limit = 0;

static uint32_t *primes = NULL;
static size_t primes_len = 0;

static uint32_t *spf = NULL;            /* smallest prime factor array */
static seq_t   *pi_vals = NULL;          /* prefix prime counts */

static seq_t   *a_cache = NULL;          /* cache of a(n) */
static size_t  a_cache_len = 0;

/* ---------- recompute all data for a new sieve limit ---------- */
static void recompute(idx_t limit)
{
    /* free previous allocations */
    free(primes);  primes = NULL; primes_len = 0;
    free(spf);     spf = NULL;
    free(pi_vals); pi_vals = NULL;
    free(a_cache); a_cache = NULL;    a_cache_len = 0;

    /* ---- compute small prime factors via linear sieve ---- */
    spf = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
    if (!spf) exit(1);
    memset(spf, 0, (limit + 1) * sizeof(uint32_t));

    size_t primes_cap = (size_t)(limit / 10) + 10;
    primes = (uint32_t *)malloc(primes_cap * sizeof(uint32_t));
    if (!primes) exit(1);

    for (size_t i = 2; i <= (size_t)limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (primes_len == primes_cap) {
                primes_cap *= 2;
                primes = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
            }
            primes[primes_len++] = (uint32_t)i;
        }
        for (size_t j = 0; j < primes_len &&
             primes[j] <= spf[i] &&
             (idx_t)i * (idx_t)primes[j] <= limit; ++j) {
            idx_t prod = (idx_t)i * (idx_t)primes[j];
            if (spf[prod] == 0) spf[prod] = primes[j];
        }
    }

    /* ---- prefix prime counts ---- */
    pi_vals = (seq_t *)malloc((limit + 1) * sizeof(seq_t));
    if (!pi_vals) exit(1);
    pi_vals[0] = 0;
    pi_vals[1] = 0;
    seq_t cnt = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i) {
        if (spf[i] == (uint32_t)i) ++cnt;
        pi_vals[i] = cnt;
    }

    /* ---- compute a_cache: number of primes <= each prime power ---- */
    size_t estimate = limit / 2 + 10;                /* rough upper bound */
    a_cache = (seq_t *)malloc(estimate * sizeof(seq_t));
    if (!a_cache) exit(1);

    for (size_t x = 2; x <= (size_t)limit; ++x) {
        uint32_t p = spf[x];
        size_t temp = x;
        while (temp % p == 0) temp /= p;
        if (temp == 1) {                                /* prime power */
            if (a_cache_len == estimate) {
                estimate *= 2;
                a_cache = (seq_t *)realloc(a_cache, estimate * sizeof(seq_t));
            }
            a_cache[a_cache_len++] = pi_vals[x];
        }
    }

    /* shrink to the actual size */
    a_cache = (seq_t *)realloc(a_cache, a_cache_len * sizeof(seq_t));

    sieve_limit = limit;
}

/* ---------- ensure at least n terms are computed ---------- */
static void ensure_cache(idx_t n)
{
    if (!initialized) {
        recompute(200000);            /* initial sieve limit */
        initialized = true;
    }
    if (a_cache_len < (size_t)n) {
        idx_t new_limit = sieve_limit;
        if (new_limit == 0) new_limit = 200;
        while (a_cache_len < (size_t)n) new_limit *= 2;
        recompute(new_limit);
    }
}

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n<1 */
    ensure_cache(n);
    return a_cache[n - 1];
}
