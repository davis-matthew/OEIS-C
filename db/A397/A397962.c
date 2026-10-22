/*
 * A397962  Complement of A397805: numbers that have fewer than 3 divisors
 *          that are smaller than the square of their least prime factor.
 * https://oeis.org/A397962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth natural number that has fewer than 3 divisors smaller
 *            than the square of its least prime factor; equivalently the
 *            number is a prime power or its second distinct prime factor is
 *            at least the square of its least prime factor
 * Range:     u8 n <= 178, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define LIM 200000          /* enough for n up to about 10000 */

/* Least‑prime‑factor sieve */
static uint32_t *spf = NULL; /* spf[i] = least prime factor of i, 0 for i<2 */
static uint32_t *prime_list = NULL;
static size_t prime_cnt = 0;

/* Cache for the sequence */
static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;
static uint64_t last_candidate = 0;

/* initialise spf and prime list once */
static void init_spf(void)
{
    if (spf != NULL)
        return;                  /* already initialized */

    spf = (uint32_t *)malloc((LIM + 1) * sizeof(uint32_t));
    if (!spf)
        exit(1);

    for (size_t i = 0; i <= LIM; ++i)
        spf[i] = (uint32_t)i;     /* initialise each entry to itself */

    for (size_t i = 2; i * i <= LIM; ++i) {
        if (spf[i] == i) {        /* i is prime */
            for (size_t j = i * i; j <= LIM; j += i) {
                if (spf[j] == j)
                    spf[j] = (uint32_t)i;
            }
        }
    }

    /* build prime list */
    prime_cnt = 0;
    for (size_t i = 2; i <= LIM; ++i)
        if (spf[i] == i)
            ++prime_cnt;

    prime_list = (uint32_t *)malloc(prime_cnt * sizeof(uint32_t));
    if (!prime_list)
        exit(1);

    size_t idx = 0;
    for (size_t i = 2; i <= LIM; ++i)
        if (spf[i] == i)
            prime_list[idx++] = (uint32_t)i;
}

/* test whether m is a member of the sequence */
static bool qualifies(uint64_t m)
{
    if (m == 1)
        return true;          /* 1 is always included */

    if (m > LIM)               /* safety: everything we test is <= LIM */
        return true;          /* fallback; should not happen */

    uint32_t p = spf[(size_t)m];

    if (p == (uint32_t)m)      /* m is prime: prime power */
        return true;

    /* strip all powers of p */
    uint64_t temp = m;
    while (temp % p == 0)
        temp /= p;

    if (temp == 1)             /* prime power (p^k) */
        return true;

    /* second distinct prime factor is spf[temp] */
    uint32_t q = spf[(size_t)temp];

    uint64_t p2 = (uint64_t)p * (uint64_t)p;
    return q >= p2;            /* include if second prime >= p^2 */
}

/* generate the nth term */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;              /* undefined, but harmless */

    idx_t target = n - 1;       /* zero‑based index into cache */

    init_spf();

    while (cache_len <= target) {
        ++last_candidate;
        if (qualifies((uint64_t)last_candidate)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    exit(1);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)last_candidate;
        }
    }
    return cache[target];
}
