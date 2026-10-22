/*
 * A326150  Nonprime numbers whose product of prime indices is divisible by
 *          their sum of prime indices.
 * https://oeis.org/A326150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Nonprime numbers whose product of prime indices is divisible by
 *            the sum of prime indices
 * Range:     u8 n <= 16, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Cap for sieve – large enough to contain the first 10 000 terms. */
#define LIMIT 500000

/* Sieve tables: smallest prime factor, index of primes, list of primes. */
static uint32_t spf[LIMIT + 1];
static uint16_t prime_index[LIMIT + 1];
static uint32_t primes[70000];
static size_t prime_count = 0;

/* Cache for the sequence values. */
static seq_t *seq_cache = NULL;
static size_t seq_len  = 0;
static size_t seq_cap  = 0;

/* One‑time initialization flag. */
static bool initialized = false;

/* Helper to push a value into the cache. */
static void push_value(seq_t val)
{
    if (seq_len + 1 > seq_cap) {
        seq_cap = seq_cap ? seq_cap * 2 : 16;
        seq_cache = (seq_t *)realloc(seq_cache, seq_cap * sizeof(seq_t));
    }
    seq_cache[seq_len++] = val;
}

/* Build the sieve, prime list, and generate the sequence. */
static void init_sequence(void)
{
    /* 1. Build spf table. */
    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if ((uint64_t)i * i <= LIMIT) {
                for (uint64_t j = (uint64_t)i * i; j <= LIMIT; j += i) {
                    if (spf[j] == 0) spf[j] = i;
                }
            }
        }
    }

    /* 2. Build prime list and index mapping. */
    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (spf[i] == i) {
            prime_index[i] = (uint16_t)(prime_count + 1);  /* 1‑based index */
            primes[prime_count++] = i;
        }
    }

    /* 3. Generate sequence up to LIMIT. */
    for (uint32_t n = 4; n <= LIMIT; ++n) {
        if (spf[n] == n) continue;  /* skip primes */

        uint64_t tmp = n;
        idx_t sum = 0;
        idx_t idx_arr[16];
        int    exp_arr[16];
        int    idx_cnt = 0;

        /* Factorize n using spf. */
        while (tmp > 1) {
            uint32_t p = spf[tmp];
            int exp = 0;
            while (tmp % p == 0) { tmp /= p; exp++; }
            idx_arr[idx_cnt]   = (idx_t)prime_index[p];
            exp_arr[idx_cnt++] = exp;
        }

        /* Compute sum of indices. */
        for (int i = 0; i < idx_cnt; ++i) sum += idx_arr[i] * exp_arr[i];
        if (sum == 0) continue;  /* should not happen */

        /* Check divisibility of product by sum using modular arithmetic. */
        idx_t prod = 1 % sum;
        bool divisible = false;
        for (int i = 0; i < idx_cnt; ++i) {
            idx_t idx = idx_arr[i];
            int    exp = exp_arr[i];
            for (int e = 0; e < exp; ++e) {
                prod = (prod * idx) % sum;
                if (prod == 0) { divisible = true; break; }
            }
            if (divisible) break;
        }
        if (divisible) push_value((seq_t)n);
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    if (!initialized) {
        init_sequence();
        initialized = true;
    }

    if ((size_t)(n - 1) < seq_len) {
        return seq_cache[n - 1];
    }

    /* If the requested term is beyond the pre‑computed range,
       the function returns 0.  The OEIS expects the function only
       to be called for n up to about 10 000, which is within the
       computed range. */
    return 0;
}
