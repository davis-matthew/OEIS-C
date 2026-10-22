/*
 * A333235  a(n) is the product of indices of unitary prime power divisors of
 *          n.
 * https://oeis.org/A333235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, look
 * Formula:   a(n) = Π_{p^k‖n} A025528(p^k) (product of OEIS indices of
 *            unitary prime powers)
 * Range:     u8 n <= 679, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* ---------- static tables (re‑built lazily) ---------- */
static uint32_t *pp_index  = NULL;   /* pp_index[v] = index of v in the sequence of unitary prime powers */
static uint64_t *prime_list = NULL;  /* prime numbers up to current limit */
static size_t    prime_cnt   = 0;
static size_t    pp_limit    = 0;     /* highest number for which tables are built */

/* ---------- helper for sorting a uint64_t array ---------- */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    return (va < vb) ? -1 : (va > vb);
}

/* ---------- build tables up to at least ‘limit’ ---------- */
static void build_tables(size_t limit)
{
    /* free old tables */
    if (prime_list != NULL) free(prime_list);
    if (pp_index   != NULL) free(pp_index);

    /* 1. sieve primes up to ‘limit’ */
    uint8_t *is_comp = (uint8_t *)calloc(limit + 1, 1);
    if (!is_comp) abort();

    size_t  cap = 1024;
    prime_list = (uint64_t *)malloc(cap * sizeof(uint64_t));
    if (!prime_list) abort();

    prime_cnt = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            if (prime_cnt == cap) {
                cap <<= 1;
                uint64_t *tmp = (uint64_t *)realloc(prime_list, cap * sizeof(uint64_t));
                if (!tmp) abort();
                prime_list = tmp;
            }
            prime_list[prime_cnt++] = i;
            if (i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i)
                    is_comp[j] = 1;
            }
        }
    }
    free(is_comp);

    /* 2. generate all prime powers ≤ limit */
    uint64_t *powers = NULL;
    size_t    pow_cap = 1024, pow_cnt = 0;
    powers = (uint64_t *)malloc(pow_cap * sizeof(uint64_t));
    if (!powers) abort();

    for (size_t idx = 0; idx < prime_cnt; ++idx) {
        uint64_t p = prime_list[idx];
        uint64_t v = p;
        while (v <= limit) {
            if (pow_cnt == pow_cap) {
                pow_cap <<= 1;
                uint64_t *tmpp = (uint64_t *)realloc(powers, pow_cap * sizeof(uint64_t));
                if (!tmpp) abort();
                powers = tmpp;
            }
            powers[pow_cnt++] = v;
            if (v > limit / p) break;
            v *= p;
        }
    }

    /* 3. sort the prime powers */
    qsort(powers, pow_cnt, sizeof(uint64_t), cmp_uint64);

    /* 4. build index table (value → index) */
    pp_index = (uint32_t *)calloc(limit + 1, sizeof(uint32_t));
    if (!pp_index) abort();
    for (size_t i = 0; i < pow_cnt; ++i)
        pp_index[powers[i]] = (uint32_t)(i + 1);

    free(powers);
    pp_limit = limit;
}

/* ------------------------------------------------------------------ */
/* Main function computing A333235   (seq_t a(idx_t n))                */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n <= 1u)                      /* a(1) = 1 */
        return (seq_t)1u;

    size_t req = (size_t)n;
    if (req > pp_limit)
        build_tables(req);

    uint64_t m   = (uint64_t)n;
    seq_t    prod = (seq_t)1u;

    for (size_t i = 0; i < prime_cnt && (uint64_t)prime_list[i] * prime_list[i] <= m; ++i) {
        uint64_t p = prime_list[i];
        if (m % p == 0) {
            uint64_t pow_val = 1;
            while (m % p == 0) {
                m /= p;
                pow_val *= p;
            }
            uint32_t idx = pp_index[pow_val];
            prod *= (seq_t)idx;
        }
    }

    if (m > 1) {                      /* remaining prime factor (exponent 1) */
        uint32_t idx = pp_index[m];
        prod *= (seq_t)idx;
    }

    return prod;
}
