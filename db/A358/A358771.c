/*
 * A358771  a(n) = 1 if the arithmetic derivative of n is of the form 4k+1,
 *          otherwise 0.
 * https://oeis.org/A358771
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the arithmetic derivative of n is ≡ 1 (mod 4),
 *            otherwise 0. The arithmetic derivative D(n) for n > 1 with prime
 *            factorisation n = ∏ p_i^e_i is D(n) = n * Σ (e_i / p_i). For n =
 *            0 or 1 the derivative is 0
 * Range:     u8 n <= 9838, u16 n <= 9838, u32 n <= 9838, u64 n <= 9838,
 *            u128 n <= 9838, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Helper: compute integer square root of a 64‑bit unsigned integer. */
static inline uint64_t isqrt64(uint64_t n)
{
    uint64_t x = n;
    uint64_t y = (x + 1) >> 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

seq_t a(idx_t n)
{
    /* n = 0 or 1 → derivative is 0 → return 0. */
    if (n <= 1)
        return 0;

    uint64_t N = (uint64_t)n;
    uint64_t tmp = N;
    uint64_t sum_mod4 = 0;

    /* handle factor 2 */
    uint64_t exp = 0;
    while ((tmp & 1ULL) == 0) {
        tmp >>= 1;
        ++exp;
    }
    if (exp) {
        uint64_t term = (exp % 4ULL) * ((N / 2ULL) % 4ULL);
        sum_mod4 = (sum_mod4 + (term % 4ULL)) % 4ULL;
    }

    /* handle odd factors up to sqrt(tmp) */
    for (uint64_t p = 3; p <= isqrt64(tmp); p += 2) {
        if (tmp % p != 0)
            continue;
        exp = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++exp;
        }
        uint64_t term = (exp % 4ULL) * ((N / p) % 4ULL);
        sum_mod4 = (sum_mod4 + (term % 4ULL)) % 4ULL;
    }

    /* If anything remains, it is a prime factor > sqrt(N). */
    if (tmp > 1) {
        /* exponent is 1 */
        uint64_t term = (1ULL % 4ULL) * ((N / tmp) % 4ULL);
        sum_mod4 = (sum_mod4 + (term % 4ULL)) % 4ULL;
    }

    return (seq_t)(sum_mod4 == 1ULL);
}
