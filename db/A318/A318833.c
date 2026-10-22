/*
 * A318833  a(n) = n + A023900(n).
 * https://oeis.org/A318833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + A023900(n), where A023900(n) = (-1)^ω(n) * φ(n) /
 *            d(n), ω(n) = number of distinct prime factors of n, d(n) = ∏
 *            p_i^{e_i-1} = n / rad(n), rad(n) the squarefree kernel
 * Range:     u8 n <= 142, u16 n <= 9948, u32 n <= 9948, u64 n <= 9948,
 *            u128 n <= 9948, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t temp = n;
    seq_t rad = 1;          // product of distinct primes
    seq_t d   = 1;          // n / rad(n)
    seq_t phi = 1;          // Euler's totient
    idx_t  omega = 0;       // number of distinct prime factors

    /* factor 2 */
    if ((temp & 1ULL) == 0) {
        idx_t e = 0;
        while ((temp & 1ULL) == 0) {
            temp >>= 1;
            e++;
        }
        if (e > 0) {
            omega++;
            rad *= 2;
            if (e > 1) {
                seq_t powp = 1;
                for (idx_t i = 1; i < e; ++i) powp *= 2;
                phi *= (2 - 1);
                phi *= powp;
                d   *= powp;
            } else {
                phi *= (2 - 1);
            }
        }
    }

    /* odd prime factors */
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p != 0) continue;
        idx_t e = 0;
        while (temp % p == 0) {
            temp /= p;
            e++;
        }
        omega++;
        rad *= p;
        seq_t powp = 1;
        for (idx_t i = 1; i < e; ++i) powp *= p;
        phi *= (seq_t)(p - 1);
        phi *= powp;
        if (e > 1) {
            d *= powp;
        }
    }

    /* remaining prime > sqrt(n) */
    if (temp > 1) {
        omega++;
        rad *= temp;
        phi *= (seq_t)(temp - 1);
        /* e = 1 -> d unchanged */
    }

    /* sign factor (-1)^ω */
    int sign = (omega % 2 == 0) ? 1 : -1;

    /* φ(n) / d(n) is an integer */
    seq_t phi_div_d = phi / d;

    /* Compute a(n) = n + sign * (φ/d) safely */
    __int128_t part = (sign == 1) ? (__int128_t)phi_div_d
                                   : -(__int128_t)phi_div_d;
    __int128_t result = (__int128_t)n + part;

    return (seq_t)result;
}
