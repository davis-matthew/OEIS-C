/*
 * A259847  Number of steps to get from n to 1 by process of replacing with
 *          the Carmichael function lambda if the result is odd, or dividing
 *          by 2 if even.
 * https://oeis.org/A259847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A259847:
   a(n) = number of steps to reduce n to 1 by:
          if n even → n = n/2,
          if n odd  → n = λ(n)   (Carmichael function)             */

#include <stdint.h>
#include <stdbool.h>

/* ----- helper: GCD for uint64_t ----- */
static inline uint64_t gcd64(uint64_t a, uint64_t b)
{
    while (b) {
        uint64_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* ----- helper: Carmichael λ for uint64_t ----- */
static seq_t lambda_u64(uint64_t n)
{
    if (n <= 1) return 1;
    if (n == 2) return 1;

    uint64_t result = 1;
    uint64_t t = n;

    /* handle factor 2 separately */
    if ((t & 1) == 0) {
        unsigned exp = 0;
        while ((t & 1) == 0) {
            t >>= 1;
            ++exp;
        }
        uint64_t lam;
        if (exp == 1)       lam = 1;            /* λ(2)   = 1   */
        else if (exp == 2)  lam = 2;            /* λ(4)   = 2   */
        else                lam = 1ULL << (exp - 2); /* λ(2^a)=2^(a-2), a≥3 */
        result = result / gcd64(result, lam) * lam;
    }

    /* factor odd prime powers */
    for (uint64_t p = 3; p * p <= t; p += 2) {
        if (t % p == 0) {
            unsigned exp = 0;
            uint64_t pp = 1;   /* p^exp */
            while (t % p == 0) {
                t /= p;
                ++exp;
                pp *= p;
            }
            /* λ(p^exp) = (p‑1) * p^(exp‑1) = (p‑1) * pp / p */
            uint64_t lam = (p - 1) * (pp / p);
            result = result / gcd64(result, lam) * lam;
        }
    }

    /* if a prime factor remains */
    if (t > 1) {
        uint64_t lam = t - 1;   /* λ(t) = φ(t) for odd prime t */
        result = result / gcd64(result, lam) * lam;
    }

    return (seq_t)result;
}

/* ----- main sequence function ----- */
seq_t a(idx_t n)
{
    uint64_t cur = (uint64_t)n;   /* all test values fit in 64 bits */
    uint64_t steps = 0;

    while (cur > 1U) {
        if ((cur & 1) == 0)          /* even */
            cur >>= 1;
        else
            cur = (uint64_t)lambda_u64(cur);
        ++steps;
    }

    return (seq_t)steps;
}
