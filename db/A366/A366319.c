/*
 * A366319  Numbers k such that the sum of prime indices of k is not twice the
 *          maximum prime index of k, meaning A056239(k) != 2 * A061395(k).
 * https://oeis.org/A366319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer k such that sum of prime indices of k *
 *            != 2 * maximum prime index of k * (A366319)
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/* Prime sieve parameters */
#define PRIME_LIMIT 1048576  /* 2^20, enough for k up to ~10^6 */
static bool sieve_initialized = false;
static uint32_t primes[PRIME_LIMIT + 1];   /* primes[0..prime_cnt-1] */
static size_t prime_cnt = 0;
static uint32_t prime_index[PRIME_LIMIT + 1]; /* prime_index[p] = index of p (1-based), 0 if not prime */

/* Initialise primes and index lookup tables */
static void init_primes(void)
{
    if (sieve_initialized) return;
    sieve_initialized = true;
    /* initialise all as prime */
    for (size_t i = 2; i <= PRIME_LIMIT; ++i) primes[i] = i;
    primes[0] = primes[1] = 0;

    for (size_t i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (primes[i]) {
            for (size_t j = i * i; j <= PRIME_LIMIT; j += i) {
                primes[j] = 0;
            }
        }
    }
    /* collect primes and indices */
    size_t idx = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (primes[i]) {
            primes[idx++] = (uint32_t)i;
        }
    }
    prime_cnt = idx;
    /* build index lookup */
    for (size_t i = 0; i < prime_cnt; ++i) {
        prime_index[primes[i]] = (uint32_t)(i + 1); /* 1-based */
    }
}

/* Storage for sequence values */
#ifdef __cplusplus
static std::vector<seq_t> seq_vec;
#else
static seq_t *seq_arr = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;
#endif

/* Push a value to the sequence storage */
static void push_seq(seq_t v)
{
#ifdef __cplusplus
    seq_vec.push_back(v);
#else
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        seq_arr = tmp;
        seq_cap = new_cap;
    }
    seq_arr[seq_len++] = v;
#endif
}

/* Main sequence function */
seq_t a(idx_t n)
{
    static idx_t next_test = 1;
    init_primes();

    /* Generate terms until we have at least n terms */
    while (true) {
#ifdef __cplusplus
        size_t len = seq_vec.size();
#else
        size_t len = seq_len;
#endif
        if (len >= n) break;

        idx_t k = next_test++;
        seq_t value = (seq_t)k;  /* candidate value */

        bool include = true;  /* default for k == 1 or if condition holds */

        if (k != 1) {
            uint64_t sum_indices = 0;
            uint32_t max_index = 0;
            idx_t temp = k;
            for (size_t i = 0; i < prime_cnt; ++i) {
                uint32_t p = primes[i];
                if ((uint64_t)p * p > (uint64_t)temp) break;
                if (temp % p == 0) {
                    uint32_t exp = 0;
                    while (temp % p == 0) {
                        temp /= p;
                        ++exp;
                    }
                    uint32_t idx = prime_index[p];
                    sum_indices += (uint64_t)idx * exp;
                    if (idx > max_index) max_index = idx;
                }
            }
            if (temp > 1) {
                /* temp is prime */
                uint32_t idx = prime_index[(uint32_t)temp];
                sum_indices += idx;          /* exponent 1 */
                if (idx > max_index) max_index = idx;
            }
            if (sum_indices == ((uint64_t)max_index << 1))
                include = false;
        }
        if (include)
            push_seq(value);
    }

    /* Return the desired term */
#ifdef __cplusplus
    return seq_vec[n - 1];
#else
    return seq_arr[n - 1];
#endif
}
