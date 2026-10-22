/*
 * A341125  Number of partitions of n into 7 prime powers (including 1).
 * https://oeis.org/A341125
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into 7 prime powers (including
 *            1)
 * Range:     u8 n <= 30, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses a dynamic–programming approach that counts unordered
   sums of exactly 7 elements taken from the multiset of all prime powers
   ≤ n (1 is treated as a prime power).  It is fully self‑contained
   and works for any seq_t defined by the build system.  */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 7u) return (seq_t)0;                /* offset starts at 7 */

    /* ---------- generate primes up to n --------------------------------- */
    idx_t limit = n;
    size_t sz = (size_t)(limit + 1);
    uint8_t *is_prime = (uint8_t*)malloc(sz);
    if (!is_prime) return (seq_t)0;              /* allocation failure */
    for (size_t i = 0; i < sz; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (idx_t i = 2; i * i <= limit; ++i)
        if (is_prime[(size_t)i])
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[(size_t)j] = 0;

    /* count primes to allocate array */
    size_t prime_count = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[(size_t)i]) ++prime_count;

    uint64_t *primes = NULL;
    if (prime_count > 0) {
        primes = (uint64_t*)malloc(prime_count * sizeof(uint64_t));
        size_t pidx = 0;
        for (idx_t i = 2; i <= limit; ++i)
            if (is_prime[(size_t)i]) primes[pidx++] = (uint64_t)i;
    }
    free(is_prime);

    /* ---------- construct prime powers (including 1) ---------------------- */
    size_t pp_capacity = (size_t)(limit + 1);   /* safe over‑estimate */
    uint64_t *prime_powers = (uint64_t*)malloc(pp_capacity * sizeof(uint64_t));
    if (!prime_powers) { free(primes); return (seq_t)0; }
    size_t pp_cnt = 0;
    prime_powers[pp_cnt++] = 1;                 /* 1 counts as a prime power */

    for (size_t pi = 0; pi < prime_count; ++pi) {
        uint64_t p = primes[pi];
        uint64_t val = p;
        while (val <= limit) {
            prime_powers[pp_cnt++] = val;
            if (val > limit / p) break;         /* avoid overflow          */
            val *= p;
        }
    }
    free(primes);

    /* ---------- dynamic programming ------------------------------------- */
    size_t dp_len = (size_t)(limit + 1) * 8;     /* columns 0..7 */
    seq_t *dp = (seq_t*)calloc(dp_len, sizeof(seq_t));
    if (!dp) { free(prime_powers); return (seq_t)0; }
    dp[0] = (seq_t)1;                           /* dp[0][0] = 1 */

    for (size_t i = 0; i < pp_cnt; ++i) {
        idx_t p = (idx_t)prime_powers[i];
        for (idx_t s = p; s <= limit; ++s) {
            size_t base  = (size_t)s * 8;
            size_t prev  = (size_t)(s - p) * 8;
            for (size_t k = 1; k <= 7; ++k)
                dp[base + k] += dp[prev + (k - 1)];
        }
    }

    seq_t result = dp[(size_t)limit * 8 + 7];
    free(dp);
    free(prime_powers);
    return result;
}
