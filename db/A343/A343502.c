/*
 * A343502  Numbers k such that tau(tau(k)) and tau(k+1) are both prime, where
 *          tau is the number of divisors function.
 * https://oeis.org/A343502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 1675, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A343502: numbers k such that tau(tau(k)) and tau(k+1) are both prime.
   The function generates terms on demand by scanning integers sequentially and
   caching the results, so calls in any order are handled correctly.
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

/* ---------- prime sieve (used for divisor counting) ---------------- */

static unsigned *primes = NULL;      /* primes list                               */
static size_t primes_sz = 0;         /* number of primes in the list              */

/* Build a list of all primes <= limit.  For the usual run limit=1e6 is enough. */
static void build_primes_up_to(size_t limit)
{
    if (limit < 2) return;

    unsigned char *mark = (unsigned char *)malloc(limit + 1);
    if (!mark) abort();
    memset(mark, 0, limit + 1);

    /* Allocate enough space for all primes up to limit (≈limit/ln(limit)). */
    unsigned *pr = (unsigned *)malloc((limit / 10 + 100) * sizeof(unsigned));
    if (!pr) abort();

    size_t count = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (!mark[i]) {
            pr[count++] = (unsigned)i;
            if (i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i) mark[j] = 1;
            }
        }
    }
    free(mark);

    primes = (unsigned *)realloc(primes, count * sizeof(unsigned));
    if (!primes) abort();
    memcpy(primes, pr, count * sizeof(unsigned));
    primes_sz = count;
    free(pr);
}

/* ---------- divisor counting for 64‑bit integers -------------------- */

static uint64_t divisor_count_uint64(uint64_t n)
{
    uint64_t res = 1;
    size_t idx = 0;
    while (idx < primes_sz && (uint64_t)primes[idx] * primes[idx] <= n) {
        unsigned p = primes[idx++];
        unsigned exp = 0;
        while (n % p == 0) {
            n /= p;
            ++exp;
        }
        res *= (uint64_t)(exp + 1);
    }
    if (n > 1) res *= 2ULL;          /* Remaining prime factor            */
    return res;
}

/* ---------- small primality test for 64‑bit values ----------------- */

static bool is_prime_small(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return false;
    return true;
}

/* ---------- cache storing produced terms --------------------------- */

static seq_t *cache = NULL;      /* array of found terms */
static size_t cache_len = 0;      /* number of terms cached */
static size_t cache_cap = 0;      /* capacity of the array  */
static uint64_t last_checked = 0;/* last integer examined */

/* -------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* offset is 1, so n<=0 is undefined */

    /* initialise primes list once */
    if (primes == NULL) {
        /* 1 000 000 gives all primes needed for k ≈ 10⁸ */
        build_primes_up_to(1000000);
    }

    /* initialise cache once */
    if (cache == NULL) {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
    }

    /* accumulate terms until we have at least n of them */
    size_t goal = (size_t)n;
    while (cache_len < goal) {
        uint64_t k = last_checked + 1;

        uint64_t tau_k   = divisor_count_uint64(k);
        uint64_t tau_tau = divisor_count_uint64(tau_k);
        uint64_t tau_kp1 = divisor_count_uint64(k + 1);

        if (is_prime_small(tau_tau) && is_prime_small(tau_kp1)) {
            /* grow cache if necessary */
            if (cache_len + 1 > cache_cap) {
                cache_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
            }
            cache[cache_len++] = (seq_t)k;   /* store as seq_t */
        }
        last_checked = k;
    }
    return cache[n - 1];
}
