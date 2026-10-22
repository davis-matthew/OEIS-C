/*
 * A369361  Numbers that are not products of primorials.
 * https://oeis.org/A369361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 231, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(n) is the nth positive integer that is NOT a product of primorials.
   We generate integers starting at 3, test each for representability as a product
   of primorials, and keep those that are not. The test checks that the
   exponents of consecutive primes in the factorisation form a non‑increasing
   sequence; otherwise the number cannot be expressed as a product of primorials. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define PRIME_LIMIT 100000   /* sufficient for the expected input range */

/* global prime table */
static int primes[PRIME_LIMIT + 1];
static size_t prime_cnt = 0;

/* initialise the prime table (called lazily) */
static void init_primes(void)
{
    bool sieve[PRIME_LIMIT + 1] = { false };
    for (int i = 2; i <= PRIME_LIMIT; i++) {
        if (!sieve[i]) {
            primes[prime_cnt++] = i;
            if ((int64_t)i * i <= PRIME_LIMIT) {
                for (int j = i * i; j <= PRIME_LIMIT; j += i)
                    sieve[j] = true;
            }
        }
    }
}

/* factor a number using the precomputed prime table */
static void factor_uint64(uint64_t n, uint64_t *pr, unsigned *exp, size_t *cnt)
{
    *cnt = 0;
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * (uint64_t)primes[i] <= n; i++) {
        if (n % primes[i] == 0) {
            unsigned e = 0;
            while (n % primes[i] == 0) {
                e++;
                n /= primes[i];
            }
            pr[(*cnt)++] = primes[i];
            exp[(*cnt) - 1] = e;
        }
    }
    if (n > 1 && *cnt < 64) {           /* remaining prime factor */
        pr[(*cnt)++] = n;
        exp[(*cnt) - 1] = 1;
    }
}

/* true if m can be written as a product of one or more primorials */
static bool is_product_of_primorials(uint64_t m)
{
    if (m == 0) return false;   /* not encountered */

    uint64_t pf[64];
    unsigned exp[64];
    size_t fcnt;
    factor_uint64(m, pf, exp, &fcnt);

    /* determine the largest prime factor */
    uint64_t maxp = pf[fcnt - 1];

    /* find exponent of prime 2 */
    size_t i_f = 0;
    uint64_t prev_exp = 0;
    if (i_f < fcnt && pf[i_f] == 2) {
        prev_exp = exp[i_f];
        i_f++;
    }

    /* iterate through global primes up to maxp */
    size_t i_g = 0;
    while (i_g < prime_cnt && (uint64_t)primes[i_g] <= maxp) {
        uint64_t cur_prime = primes[i_g];
        if (cur_prime == 2) {          /* already processed */
            i_g++;
            continue;
        }
        uint64_t cur_exp = 0;
        if (i_f < fcnt && pf[i_f] == cur_prime) {
            cur_exp = exp[i_f];
            i_f++;
        }
        if (cur_exp > prev_exp) return false;  /* violates non‑increasing property */
        prev_exp = cur_exp;
        i_g++;
    }
    return true;   /* exponents form a non‑increasing sequence */
}

/* dynamic cache for sequence terms */
typedef struct {
    seq_t *data;
    size_t len;
    size_t cap;
    uint64_t last_val;   /* last integer examined */
} seq_cache_t;

static seq_cache_t cache = { NULL, 0, 0, 1 };   /* start from 1 before first test */

static void ensure_cache_capacity(size_t need)
{
    if (need <= cache.cap) return;
    size_t new_cap = cache.cap ? cache.cap * 2 : 4;
    while (new_cap < need) new_cap *= 2;
    seq_t *new_data = (seq_t *)realloc(cache.data, new_cap * sizeof(seq_t));
    if (!new_data) abort();
    cache.data = new_data;
    cache.cap = new_cap;
}

seq_t a(idx_t n)
{
    if (!prime_cnt) init_primes();   /* initialise primes once */

    if (n == 0) return 0;            /* undefined for n<1, but never called */

    /* extend cache until we have n terms */
    while (cache.len < n) {
        cache.last_val++;
        uint64_t val = cache.last_val;
        if (val < 3) continue;        /* skip 1 and 2 (not in sequence) */
        if (!is_product_of_primorials(val)) {
            /* add to cache */
            ensure_cache_capacity(cache.len + 1);
            cache.data[cache.len++] = (seq_t)val;
        }
    }
    return cache.data[n - 1];
}
