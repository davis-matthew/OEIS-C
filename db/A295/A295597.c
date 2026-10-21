/*
 * A295597  Numbers k such that Bernoulli number B_{k} has denominator
 *          4501770.
 * https://oeis.org/A295597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = numbers k such that the denominator of Bernoulli B_k is
 *            4501770. * Denominator of B_k is the product of all primes p
 *            with (p-1)|k. * For this denominator the required primes are
 *            2,3,5,7,13,17,97. * Hence k must be 96·m and for every prime p ≠
 *            {2,3,5,7,13,17,97} * if (p-1)|(96·m) then k is...
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* ----------  Constant data  ---------- */
static const uint32_t REQUIRED_PRIMES[] = {2,3,5,7,13,17,97};
static const size_t REQUIRED_COUNT = 7;

/* ----------  Prime sieve (up to 10000)  ---------- */
static uint32_t *prime_list = NULL;
static size_t prime_count = 0;

static void init_primes(void) {
    if (prime_count) return;   /* already built */
    const uint32_t MAXP = 10000;
    unsigned char is_composite[MAXP + 1];
    for (uint32_t i = 0; i <= MAXP; ++i) is_composite[i] = 0;
    for (uint32_t i = 2; (uint64_t)i * i <= MAXP; ++i) {
        if (!is_composite[i]) {
            for (uint32_t j = i * i; j <= MAXP; j += i) is_composite[j] = 1;
        }
    }
    /* count primes */
    for (uint32_t i = 2; i <= MAXP; ++i) if (!is_composite[i]) ++prime_count;
    prime_list = (uint32_t*)malloc(prime_count * sizeof(uint32_t));
    size_t idx = 0;
    for (uint32_t i = 2; i <= MAXP; ++i) if (!is_composite[i]) prime_list[idx++] = i;
}

/* ----------  Utilities  ---------- */
static int is_prime_uint32(uint32_t n) {
    if (n < 2) return 0;
    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = prime_list[i];
        if ((uint64_t)p * p > n) break;
        if (n % p == 0) return 0;
    }
    return 1;
}

static int in_required_set(uint32_t p) {
    for (size_t i = 0; i < REQUIRED_COUNT; ++i) {
        if (REQUIRED_PRIMES[i] == p) return 1;
    }
    return 0;
}

/* ----------  Cache for sequence values  ---------- */
static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;
static idx_t last_m = 0;   /* last m value tested (not necessarily stored) */

static void grow_cache(idx_t new_cap) {
    if (new_cap <= cache_cap) return;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) { /* unrecoverable allocation error */
        exit(1);
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* ----------  Main function  ---------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;              /* offset is 1 */

    if (cache_len >= n) return cache[n-1];

    init_primes();                     /* build primes once */

    if (cache_cap < n) grow_cache(n);

    /* Generate terms until we have n entries */
    idx_t m = last_m + 1;
    while (cache_len < n) {
        uint64_t k = 96ULL * (uint64_t)m;

        /* factor k */
        uint32_t fac_pr[12];
        uint8_t  fac_exp[12];
        size_t   fac_cnt = 0;
        uint64_t tmp = k;
        for (size_t i = 0; i < prime_count; ++i) {
            uint32_t p = prime_list[i];
            if ((uint64_t)p * p > tmp) break;
            if (tmp % p == 0) {
                uint8_t e = 0;
                while (tmp % p == 0) { tmp /= p; ++e; }
                fac_pr[fac_cnt] = p;
                fac_exp[fac_cnt] = e;
                ++fac_cnt;
            }
        }
        if (tmp > 1) {    /* remaining prime factor > 10000 */
            fac_pr[fac_cnt] = (uint32_t)tmp;
            fac_exp[fac_cnt] = 1;
            ++fac_cnt;
        }

        /* enumerate all divisors of k */
        uint64_t divisors[4096];   /* safe upper bound for divisor count */
        size_t div_cnt = 1;
        divisors[0] = 1;
        for (size_t fi = 0; fi < fac_cnt; ++fi) {
            uint32_t p = fac_pr[fi];
            uint8_t  e = fac_exp[fi];
            size_t start = div_cnt;
            uint64_t mult = 1;
            for (uint8_t ee = 1; ee <= e; ++ee) {
                mult *= p;
                for (size_t di = 0; di < start; ++di) {
                    divisors[div_cnt++] = divisors[di] * mult;
                }
            }
        }

        /* check admissibility */
        int ok = 1;
        for (size_t di = 0; di < div_cnt && ok; ++di) {
            uint64_t d = divisors[di];
            uint32_t p = (uint32_t)(d + 1);
            if (p < 2) continue;
            if (!is_prime_uint32(p)) continue;          /* not a prime */
            if (!in_required_set(p)) { ok = 0; break; }  /* disallowed prime */
        }

        if (ok) {
            cache[cache_len++] = (seq_t)k;
            last_m = m;
        }

        ++m;
    }

    return cache[n-1];
}
