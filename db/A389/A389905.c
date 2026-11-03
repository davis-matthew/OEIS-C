/*
 * A389905  a(n) = 1 if n is neither squarefree nor powerful, otherwise 0.
 * https://oeis.org/A389905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n is neither squarefree nor powerful, otherwise 0;
 *            a(n) = 0 for n=1
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 136 terms through n = 9994 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* simple prime sieve up to limit (inclusive) */
static void sieve_primes_upto(idx_t limit, uint64_t **prime_arr, size_t *cnt)
{
    if (limit < 2) {
        *prime_arr = NULL;
        *cnt = 0;
        return;
    }

    size_t sz = (size_t)limit + 1;
    uint8_t *is_comp = (uint8_t *)calloc(sz, sizeof(uint8_t));
    if (!is_comp) {
        *prime_arr = NULL;
        *cnt = 0;
        return;
    }

    size_t est = (size_t)(limit / 2 + 10);
    uint64_t *pr = (uint64_t *)malloc(est * sizeof(uint64_t));
    if (!pr) {
        free(is_comp);
        *prime_arr = NULL;
        *cnt = 0;
        return;
    }

    size_t c = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            if (c < est)
                pr[c++] = (uint64_t)i;
            if ((idx_t)i * (idx_t)i <= limit) {
                for (idx_t j = (idx_t)i * (idx_t)i; j <= limit; j += i)
                    is_comp[j] = 1;
            }
        }
    }

    free(is_comp);
    *prime_arr = pr;
    *cnt = c;
}

/* integer square‑root (floor) using binary search */
static inline idx_t int_isqrt(idx_t n)
{
    idx_t low = 0;
    idx_t high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid == 0) {
            low = 1;
            continue;
        }
        if (mid <= n / mid) {
            /* mid^2 <= n, try larger */
            if ((mid + 1) > n / (mid + 1))
                return mid;
            low = mid + 1;
        } else {
            /* mid^2 > n */
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return n;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n <= 1)          /* 1 is both squarefree and powerful */
        return 0;

    idx_t limit = int_isqrt(n);
    size_t prime_cnt;
    uint64_t *primes;
    sieve_primes_upto(limit, &primes, &prime_cnt);

    bool hasOne = false;   /* at least one exponent equals 1 */
    bool hasGE2 = false;   /* at least one exponent >= 2 */
    idx_t temp = n;

    for (size_t i = 0; i < prime_cnt && primes[i] <= limit; ++i) {
        uint64_t p = primes[i];
        if (temp % p == 0) {
            unsigned exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            if (exp == 1)
                hasOne = true;
            else
                hasGE2 = true;

            if (hasOne && hasGE2) {
                free(primes);
                return 1;
            }
        }
    }

    if (temp > 1)         /* remaining prime factor with exponent 1 */
        hasOne = true;

    free(primes);
    return (seq_t)(hasOne && hasGE2 ? 1 : 0);
}
