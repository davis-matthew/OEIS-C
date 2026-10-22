/*
 * A349958  a(n) is the index of the first row in Pascal's triangle that
 *          contains a multiple of n.
 * https://oeis.org/A349958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the least j such that ∃k, 0≤k≤j with n | C(j,k)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation is by checking all rows `j` ≥ the largest prime power dividing n
   until one row contains a binomial coefficient divisible by `n`. This uses
   prime‑factor exponent tables for speed. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define MAX_N 10000

/* --- Utility: smallest prime factor sieve -------------------------------- */
static uint16_t spf[MAX_N + 1];
static uint32_t prime_index[MAX_N + 1];
static unsigned prime_count;

/* Build spf and prime_index */
static void build_spf(void)
{
    for (uint32_t i = 0; i <= MAX_N; ++i) {
        spf[i] = (uint16_t)i;
        prime_index[i] = 0;
    }
    for (uint32_t i = 2; i * i <= MAX_N; ++i) {
        if (spf[i] == i) {
            for (uint32_t j = i * i; j <= MAX_N; j += i) {
                if (spf[j] == j) spf[j] = i;
            }
        }
    }
    prime_count = 0;
    for (uint32_t i = 2; i <= MAX_N; ++i) {
        if (spf[i] == i) {
            ++prime_count;
            prime_index[i] = prime_count; /* 1‑based index */
        }
    }
}

/* --- Exponent tables: exp[pIdx][m] = v_p(m!) -------------------------------- */
static uint16_t **exp_table;   /* dimension [prime_count+1][MAX_N+1] */
static void build_exp_tables(void)
{
    exp_table = (uint16_t **)calloc(prime_count + 1, sizeof(uint16_t *));
    for (uint32_t i = 1; i <= prime_count; ++i) {
        exp_table[i] = (uint16_t *)calloc(MAX_N + 1, sizeof(uint16_t));
    }
    for (uint32_t m = 1; m <= MAX_N; ++m) {
        uint32_t x = m;
        while (x > 1) {
            uint32_t p = spf[x];
            uint32_t cnt = 0;
            while (x % p == 0) { x /= p; ++cnt; }
            uint32_t idx = prime_index[p];
            exp_table[idx][m] = exp_table[idx][m - 1] + cnt;
        }
        /* copy previous counts for other primes */
        for (uint32_t i = 1; i <= prime_count; ++i) {
            if (exp_table[i][m] == 0) exp_table[i][m] = exp_table[i][m - 1];
        }
    }
}

/* --- Cache for sequence values -------------------------------------------- */
static seq_t *cache_array;

/* --- Prime factorization of n (up to MAX_N) -------------------------------- */
static void factorize_uint16(uint16_t n, uint16_t primes[], uint8_t powers[],
                             uint8_t *cnt)
{
    uint8_t c = 0;
    while (n > 1) {
        uint32_t p = spf[n];
        uint8_t e = 0;
        while (n % p == 0) { n /= p; ++e; }
        primes[c] = (uint16_t)p;
        powers[c] = e;
        ++c;
    }
    *cnt = c;
}

/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Lazy static initialization */
    static bool initialized = false;
    if (!initialized) {
        build_spf();
        build_exp_tables();
        cache_array = (seq_t *)calloc(MAX_N + 1, sizeof(seq_t));
        cache_array[0] = 0;           /* unused */
        cache_array[1] = 0;           /* a(1) = 0 */
        initialized = true;
    }

    if (n <= 1) return 0;
    if (n <= MAX_N && cache_array[n] != 0) return cache_array[n];

    /* n is assumed ≤ MAX_N for this implementation */
    uint16_t primes[16];
    uint8_t powers[16];
    uint8_t pfCnt;
    factorize_uint16((uint16_t)n, primes, powers, &pfCnt);

    /* Compute largest prime power dividing n */
    idx_t start = 0;
    for (uint8_t i = 0; i < pfCnt; ++i) {
        idx_t pows = 1;
        for (uint8_t j = 0; j < powers[i]; ++j) pows *= primes[i];
        if (pows > start) start = pows;
    }
    if (start == 0) start = 1;  /* safety */

    seq_t answer = n;  /* fallback: at worst a(n) = n */
    for (idx_t m = start; m <= n; ++m) {
        /* Check k = 1 case */
        if (m % n == 0) { answer = (seq_t)m; break; }

        /* Precompute factorial exponents for m for all primes of n */
        uint16_t val_m[16];
        for (uint8_t i = 0; i < pfCnt; ++i) {
            uint32_t idx = prime_index[primes[i]];
            val_m[i] = exp_table[idx][m];
        }

        /* Search k from 2 to m-2 (k=m-1 handled by symmetry) */
        bool found = false;
        for (idx_t k = 2; k <= m - 2; ++k) {
            bool ok = true;
            for (uint8_t i = 0; i < pfCnt; ++i) {
                uint32_t idx = prime_index[primes[i]];
                uint16_t val = val_m[i] - exp_table[idx][k] - exp_table[idx][m - k];
                if (val < powers[i]) { ok = false; break; }
            }
            if (ok) {
                answer = (seq_t)m;
                found = true;
                break;
            }
        }
        if (found) break;
    }

    if (n <= MAX_N) cache_array[n] = answer;
    return answer;
}
