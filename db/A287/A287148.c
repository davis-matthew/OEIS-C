/*
 * A287148  Number of compositions (ordered partitions) of 2*n-1 into primes
 *          of form x^2 + y^2.
 * https://oeis.org/A287148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of compositions of 2*n-1 into primes p such that *
 *            p == 2 or p ≡ 1 (mod 4). Computed by dynamic programming: * f(0)
 *            = 1; * f(m) = ∑_{p ≤ m, p ∈ S} f(m-p), for m ≥ 1, * where S =
 *            {2} ∪ {p prime | p ≡ 1 (mod 4)}
 * Range:     u8 n <= 15, u16 n <= 26, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset is 1, but guard anyway */
    idx_t target = 2*n - 1;              /* size to compute */

    /* ----- 1. Sieve of Eratosthenes up to target ----- */
    bool *is_prime = (bool*)malloc((size_t)(target + 1) * sizeof(bool));
    if (!is_prime) return 0;             /* out of memory fallback */
    for (size_t i = 0; i <= (size_t)target; ++i) is_prime[i] = true;
    if (target >= 0) is_prime[0] = false;
    if (target >= 1) is_prime[1] = false;

    for (idx_t i = 2; i <= target / i; ++i) {
        if (!is_prime[(size_t)i]) continue;
        for (idx_t j = i * i; j <= target; j += i) is_prime[(size_t)j] = false;
    }

    /* ----- 2. Extract allowed primes (2 or p % 4 == 1) ----- */
    /* First count them to allocate exact size. */
    size_t allow_cnt = 0;
    for (idx_t i = 2; i <= target; ++i) {
        if (is_prime[(size_t)i] && (i == 2 || (i % 4 == 1))) allow_cnt++;
    }
    uint64_t *allow = (uint64_t*)malloc(allow_cnt * sizeof(uint64_t));
    if (!allow) { free(is_prime); return 0; }
    size_t pos = 0;
    for (idx_t i = 2; i <= target; ++i) {
        if (is_prime[(size_t)i] && (i == 2 || (i % 4 == 1))) allow[pos++] = (uint64_t)i;
    }

    /* ----- 3. Dynamic programming ----- */
    seq_t *dp = (seq_t*)malloc((size_t)(target + 1) * sizeof(seq_t));
    if (!dp) { free(is_prime); free(allow); return 0; }
    dp[0] = 1;
    for (idx_t m = 1; m <= target; ++m) {
        seq_t sum = 0;
        for (size_t k = 0; k < allow_cnt; ++k) {
            uint64_t p = allow[k];
            if (p > m) break;
            sum += dp[(size_t)(m - p)];
        }
        dp[(size_t)m] = sum;
    }

    seq_t result = dp[(size_t)target];

    /* ----- 4. Clean up ----- */
    free(is_prime);
    free(allow);
    free(dp);
    return result;
}
