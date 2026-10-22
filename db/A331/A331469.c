/*
 * A331469  a(n) is the greatest value of the form p_1 + ... + p_k where p_1,
 *          ..., p_k are powers of primes and such that the concatenation of
 *          the binary representations of p_1, ..., p_k equals the binary
 *          representation of n.
 * https://oeis.org/A331469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = greatest sum of prime powers whose binary representations
 *            concatenated together equal the binary representation of n
 * Range:     u8 n <= 255, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* helper: check whether m is a prime power (p^e with e >= 0, p prime) */
static inline bool is_prime_power(uint64_t m) {
    if (m == 1) return true;                     /* 1 = p^0 */
    if (m <= 3) return true;                     /* 2,3 are prime */

    /* check divisibility by 2 */
    if (m % 2 == 0) {
        if (m == 2) return true;
        do { m /= 2; } while (m % 2 == 0);
        return m == 1;
    }

    /* trial division by odd numbers */
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            do { m /= p; } while (m % p == 0);
            return m == 1;
        }
    }
    return true;   /* m is prime */
}

/* Compute a(n) for the OEIS sequence A331469 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safety: not used for the sequence, but protects shifts */

    /* find binary length L and store bits */
    uint64_t tmp = (uint64_t)n;
    size_t L = 0;
    while (tmp) { ++L; tmp >>= 1; }
    uint8_t bits[64];
    for (size_t i = 0; i < L; ++i) {
        bits[i] = (uint8_t)((n >> (L - 1 - i)) & 1ULL);
    }

    /* prefix values for quick substring extraction */
    uint64_t prefix[65];
    prefix[0] = 0;
    for (size_t i = 1; i <= L; ++i) {
        prefix[i] = (prefix[i - 1] << 1) | bits[i - 1];
    }

    /* DP arrays for best sums; small fixed size (L <= 64) */
    seq_t dp[65] = {0};
    bool ok[65] = {false};
    ok[0] = true;

    for (size_t i = 1; i <= L; ++i) {
        seq_t best = 0;
        bool found = false;
        for (size_t j = 0; j < i; ++j) {
            if (!ok[j] || bits[j] == 0) continue;   /* each chunk must start with '1' */
            size_t len = i - j;
            uint64_t high = (len < 64) ? (prefix[j] << len) : 0;
            uint64_t val = prefix[i] - high;
            if (!is_prime_power(val)) continue;
            seq_t cand = dp[j] + (seq_t)val;
            if (!found || cand > best) { best = cand; found = true; }
        }
        if (found) { dp[i] = best; ok[i] = true; }
    }
    return dp[L];
}
