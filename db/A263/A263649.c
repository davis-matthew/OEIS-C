/*
 * A263649  a(n) is multiplicative with a(2^e) = (-1)^e, a(3^e) = -2*(-1)^e if
 *          e>0, a(p^e) = e+1 if p == 1, 7 (mod 24), a(p^e) = (e+1) * (-1)^e
 *          if p == 5, 11 (mod 24), a(p^e) = (1 + (-1)^e) / 2 if p == 13, 17,
 *          19, 23 (mod 24).
 * https://oeis.org/A263649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   multiplicative function with prime‑power values * * a(1) = 1 *
 *            For a prime power p^e with e ≥ 1: * - p = 2 : a(2^e) = (‑1)^e *
 *            - p = 3 : a(3^e) = –2·(‑1)^e * - p ≡ 1 or 7 (mod 24) : a(p^e) =
 *            e + 1 * - p ≡ 5 or 11 (mod 24) : a(p^e) = (e + 1)·(‑1)^e * - p ≡
 *            13,17,19,23 (mod 24) : a(p^e) = (1 + (‑1)...
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;

    uint64_t nn = (uint64_t)n;
    seq_t result = 1;

    /* Handle factor 2 */
    size_t e = 0;
    while ((nn & 1ULL) == 0) { /* nn % 2 == 0 */
        nn >>= 1;              /* divide by 2 */
        ++e;
    }
    if (e > 0) {
        seq_t val = (e % 2 == 0) ? (seq_t)1 : (seq_t)-1; /* (-1)^e */
        result *= val;
        if (result == 0) return result;
    }

    /* Handle factor 3 */
    e = 0;
    while (nn % 3ULL == 0) {
        nn /= 3ULL;
        ++e;
    }
    if (e > 0) {
        seq_t val = (e % 2 == 0) ? (seq_t)-2 : (seq_t)2; /* -2*(-1)^e */
        result *= val;
        if (result == 0) return result;
    }

    /* Handle remaining odd factors */
    for (uint64_t p = 5; p * p <= nn; p += 2) {
        if (nn % p == 0) {
            e = 0;
            while (nn % p == 0) {
                nn /= p;
                ++e;
            }

            seq_t val;
            unsigned r = (unsigned)(p % 24U);
            if (r == 1 || r == 7) {
                val = (seq_t)(e + 1);                    /* e+1 */
            } else if (r == 5 || r == 11) {
                seq_t sign = (e % 2 == 0) ? (seq_t)1 : (seq_t)-1;
                val = (seq_t)(e + 1) * sign;            /* (e+1)*(-1)^e */
            } else { /* 13,17,19,23 */
                val = (e % 2 == 0) ? (seq_t)1 : (seq_t)0; /* (1+(-1)^e)/2 */
            }
            result *= val;
            if (result == 0) return result;
        }
    }

    /* If a prime factor remains > sqrt(original n) */
    if (nn > 1) {
        uint64_t p = nn;
        e = 1; /* exponent is 1 for the remaining prime */

        seq_t val;
        unsigned r = (unsigned)(p % 24U);
        if (r == 1 || r == 7) {
            val = (seq_t)(e + 1);
        } else if (r == 5 || r == 11) {
            seq_t sign = (e % 2 == 0) ? (seq_t)1 : (seq_t)-1;
            val = (seq_t)(e + 1) * sign;
        } else { /* 13,17,19,23 */
            val = (e % 2 == 0) ? (seq_t)1 : (seq_t)0;
        }
        result *= val;
    }

    return result;
}
