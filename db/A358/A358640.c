/*
 * A358640  Number of partitions of n into at most 4 distinct nonprime parts.
 * https://oeis.org/A358640
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into at most 4 distinct
 *            nonprime parts
 * Range:     u8 n <= 59, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: Count subsets of size ≤4 from the set of nonprime integers ≤n
   that sum to n (each element used at most once). This is done with dynamic
   programming, once per run for all n up to a fixed maximum (10000). */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define MAX_N 10000

static bool _initialized = false;
static seq_t _dp[5 * (MAX_N + 1)];
static size_t _cnt_nonprimes = 0;
static int _nonprimes[MAX_N + 1];

/* Initialize the DP table and the list of nonprime numbers up to MAX_N. */
static void _init(void) {
    if (_initialized) return;
    _initialized = true;

    /* Sieve of Eratosthenes to find primes up to MAX_N */
    bool is_prime[MAX_N + 1];
    for (int i = 0; i <= MAX_N; ++i) is_prime[i] = true;
    if (MAX_N >= 0) is_prime[0] = false;
    if (MAX_N >= 1) is_prime[1] = false;
    for (int i = 2; i * i <= MAX_N; ++i) {
        if (is_prime[i]) {
            for (int j = i * i; j <= MAX_N; j += i) is_prime[j] = false;
        }
    }

    /* Build list of nonprime numbers (including 1). */
    for (int i = 1; i <= MAX_N; ++i) {
        if (!is_prime[i]) {
            _nonprimes[_cnt_nonprimes++] = i;
        }
    }

    /* Dynamic programming: dp[k][s] counts subsets of size k that sum to s. */
    /* Since _dp is global, it is zero-initialised. */
    _dp[0] = (seq_t)1;               /* dp[0][0] = 1 (empty subset) */

    const int stride = MAX_N + 1;
    for (size_t idx = 0; idx < _cnt_nonprimes; ++idx) {
        int x = _nonprimes[idx];
        for (int k = 4; k >= 1; --k) {
            int base_k = k * stride;
            int base_km1 = (k - 1) * stride;
            for (int s = MAX_N; s >= x; --s) {
                _dp[base_k + s] += _dp[base_km1 + (s - x)];
            }
        }
    }
}

/* Public function computing a(n). */
seq_t a(idx_t n) {
    if (n > MAX_N) {
        /* For n larger than the precomputed maximum, return 0 (unsupported). */
        return (seq_t)0;
    }
    _init();  /* Ensure the table is ready on first call. */

    seq_t result = 0;
    const int stride = MAX_N + 1;
    for (int k = 0; k <= 4; ++k) {
        result += _dp[k * stride + (int)n];
    }
    return result;
}
