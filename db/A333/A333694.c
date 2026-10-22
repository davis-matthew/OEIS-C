/*
 * A333694  Expansion of Sum_{k>=1} k * x^k / (1 - x^(k^2)).
 * https://oeis.org/A333694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{d|n, n/d == 1 (mod d)} d
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A333694: sum of divisors d of n such that n/d ≡ 1 (mod d) */

#include <stdlib.h>
#include <stdint.h>

/* Maximum number of divisors for n ≤ 10000 is 128; use a small buffer */
#define MAX_DIVISORS 256

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for 0, but safe fallback */

    /* Factorize n by trial division */
    idx_t temp = n;
    idx_t primes[16];
    uint8_t exps[16];
    size_t pcnt = 0;

    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            uint8_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[pcnt] = p;
            exps[pcnt]   = e;
            ++pcnt;
        }
    }
    if (temp > 1) {
        primes[pcnt] = temp;
        exps[pcnt]   = 1;
        ++pcnt;
    }

    /* Generate all divisors from prime factorization */
    idx_t divs[MAX_DIVISORS];
    size_t dcnt = 1;
    divs[0] = 1;

    for (size_t i = 0; i < pcnt; ++i) {
        idx_t base = 1;
        size_t old = dcnt;
        for (uint8_t k = 1; k <= exps[i]; ++k) {
            base *= primes[i];
            for (size_t j = 0; j < old; ++j) {
                divs[dcnt++] = divs[j] * base;
            }
        }
    }

    /* Sum divisors satisfying n/d ≡ 1 (mod d) */
    seq_t sum = 0;
    for (size_t i = 0; i < dcnt; ++i) {
        idx_t d = divs[i];
        idx_t nd = n / d;                 /* integer division: d | n */
        if ((nd - 1) % d == 0) {
            sum += (seq_t)d;
        }
    }

    return sum;
}
