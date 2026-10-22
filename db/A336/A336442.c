/*
 * A336442  Numbers having 3 pairwise coprime divisors, {d_1, d_2, d_3}, such
 *          that d_1 < d_2 < d_3 < 2*d_1.
 * https://oeis.org/A336442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th number that has three pairwise coprime divisors d1 <
 *            d2 < d3 < 2*d1. The function computes the sequence by a
 *            straightforward search, using trial division and divisor
 *            generation
 * Range:     u8 n <= 6, u16 n <= 4156, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard includes needed for all supported integer types */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/gcd_u64.h"

/* ---------- Utility: Euclid's gcd for uint64_t ---------- */

/* ---------- Prime sieve (fixed limit, used for trial division) ---------- */
#define PRIME_SIEVE_LIMIT 1000000U
static uint32_t *primes = NULL;   /* prime list */
static size_t primes_cnt = 0;     /* number of primes */

static void init_primes(void)
{
    if (primes != NULL) return;          /* already initialised */

    bool *is_prime = (bool*)malloc(PRIME_SIEVE_LIMIT + 1);
    if (!is_prime) exit(EXIT_FAILURE);
    memset(is_prime, 1, PRIME_SIEVE_LIMIT + 1);
    is_prime[0] = is_prime[1] = 0;

    for (uint32_t i = 2; i * i <= PRIME_SIEVE_LIMIT; ++i) {
        if (is_prime[i]) {
            for (uint32_t j = i * i; j <= PRIME_SIEVE_LIMIT; j += i)
                is_prime[j] = 0;
        }
    }

    primes = (uint32_t*)malloc(80000 * sizeof(uint32_t)); /* rough capacity for 1e6 */
    size_t idx = 0;
    for (uint32_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i)
        if (is_prime[i])
            primes[idx++] = i;
    primes_cnt = idx;

    free(is_prime);
}

/* ---------- Factorisation by trial division ---------- */
static void factor_uint64(uint64_t n,
                          uint32_t *pr, uint8_t *exp, size_t *cnt)
{
    size_t i = 0;
    for (size_t pi = 0; pi < primes_cnt && (uint64_t)primes[pi] * primes[pi] <= n; ++pi) {
        uint32_t p = primes[pi];
        if (n % p == 0) {
            exp[i] = 0;
            while (n % p == 0) {
                n /= p;
                exp[i]++;
            }
            pr[i++] = p;
        }
    }
    if (n > 1) {
        pr[i]   = (uint32_t)n;
        exp[i]  = 1;
        ++i;
    }
    *cnt = i;
}

/* ---------- Divisor generation (recursively) ---------- */
static void gen_divs_rec(const uint32_t *pr, const uint8_t *exp,
                         size_t idx, size_t cnt,
                         uint64_t cur,
                         uint64_t *divs, size_t *divcnt)
{
    if (idx == cnt) {
        divs[(*divcnt)++] = cur;
        return;
    }
    uint64_t cur_mul = cur;
    for (uint8_t e = 0; e <= exp[idx]; ++e) {
        gen_divs_rec(pr, exp, idx + 1, cnt, cur_mul, divs, divcnt);
        cur_mul *= pr[idx];
    }
}

static int cmp_uint64(const void *a, const void *b)
{
    uint64_t ua = *(const uint64_t*)a;
    uint64_t ub = *(const uint64_t*)b;
    return (ua > ub) - (ua < ub);
}

/* ---------- Test whether a number has a qualifying triple of divisors ---------- */
static bool has_triple_coprime_divisors(uint64_t m)
{
    uint32_t pr[32];
    uint8_t  ex[32];
    size_t   pcnt;
    factor_uint64(m, pr, ex, &pcnt);

    /* 2048 divisors suffice for numbers that occur in this sequence up to the limits tested */
    uint64_t divs[2048];
    size_t   dcnt = 0;
    gen_divs_rec(pr, ex, 0, pcnt, 1, divs, &dcnt);

    qsort(divs, dcnt, sizeof(uint64_t), cmp_uint64);

    for (size_t i = 0; i + 2 < dcnt; ++i) {
        uint64_t d1 = divs[i];
        for (size_t j = i + 1; j + 1 < dcnt; ++j) {
            uint64_t d2 = divs[j];
            if (gcd_u64(d1, d2) != 1) continue;
            for (size_t k = j + 1; k < dcnt; ++k) {
                uint64_t d3 = divs[k];
                if (d3 >= 2 * d1) break;          /* divisors are sorted */
                if (gcd_u64(d1, d3) != 1) continue;
                if (gcd_u64(d2, d3) != 1) continue;
                return true;
            }
        }
    }
    return false;
}

/* ---------- Cache for found terms ---------- */
static uint64_t *cache64 = NULL;  /* cache of terms up to seq_t width */
static size_t cache_size = 0, cache_cap = 0;
static uint64_t next_candidate = 60;  /* first term in the sequence */

/* ---------- Main sequence routine ---------- */
seq_t a(idx_t n)
{
    init_primes();

    /* extend cache until we have n entries */
    while (cache_size < n) {
        if (has_triple_coprime_divisors(next_candidate)) {
            if (cache64 == NULL) {
                cache_cap = 16;
                cache64 = (uint64_t*)malloc(cache_cap * sizeof(uint64_t));
                if (!cache64) exit(EXIT_FAILURE);
            } else if (cache_size >= cache_cap) {
                cache_cap *= 2;
                cache64 = (uint64_t*)realloc(cache64, cache_cap * sizeof(uint64_t));
                if (!cache64) exit(EXIT_FAILURE);
            }
            cache64[cache_size++] = next_candidate;
        }
        ++next_candidate;
    }

    return (seq_t)cache64[n - 1];
}
