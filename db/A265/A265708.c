/*
 * A265708  a(n) = lcm_{d|n} sigma(d) * Sum_{d|n} 1/sigma(d), where sigma(d)
 *          represents the sum of divisors of d (A000203(d)).
 * https://oeis.org/A265708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = lcm_{d|n} sigma(d) * Sum_{d|n} 1/sigma(d)
 * Range:     u8 n <= 15, u16 n <= 63, u32 n <= 511, u64 n <= 8191,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 74 terms through n = 8698 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

/* Euclidean GCD for seq_t values. */
static seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* Generate all divisors of n. */
    idx_t divs[256];
    size_t dcnt = 0;
    divs[dcnt++] = 1;

    idx_t rem = n;
    for (idx_t p = 2; p * p <= rem; ++p) {
        if (rem % p == 0) {
            size_t exp = 0;
            while (rem % p == 0) {
                rem /= p;
                ++exp;
            }
            size_t curcnt = dcnt;
            for (size_t i = 0; i < curcnt; ++i) {
                idx_t base = divs[i];
                for (size_t k = 1; k <= exp; ++k) {
                    base *= p;
                    divs[dcnt++] = base;
                }
            }
        }
    }
    if (rem > 1) {
        size_t curcnt = dcnt;
        for (size_t i = 0; i < curcnt; ++i) {
            divs[dcnt++] = divs[i] * rem;
        }
    }

    /* Compute L = lcm{sigma(d)} for all divisors d|n. */
    seq_t L = 1;
    for (size_t i = 0; i < dcnt; ++i) {
        seq_t s = sigma_of(divs[i]);
        seq_t g = gcd_seq(L, s);
        L = (L / g) * s;
    }

    /* Compute result = Σ_{d|n} L / sigma(d). */
    seq_t result = 0;
    for (size_t i = 0; i < dcnt; ++i) {
        seq_t s = sigma_of(divs[i]);
        result += L / s;
    }

    return result;
}
