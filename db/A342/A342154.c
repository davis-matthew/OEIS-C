/*
 * A342154  Number of partitions of n^5 into two positive squares.
 * https://oeis.org/A342154
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of unordered positive solutions to x² + y² = n⁵ *
 *            Computation uses the factorization of n and the classical
 *            formula for * the number of representations of an integer as a
 *            sum of two squares
 * Range:     u8 n <= 3049, u16 n <= 3050, u32 n <= 3050, u64 n <= 3050,
 *            u128 n <= 3050, big any n
 * Helpers:   uint64_sqrt_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/uint64_sqrt_floor.h"

static bool is_square_uint64(uint64_t x)
{
    uint64_t r = uint64_sqrt_floor(x);
    return r * r == x;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* 0⁵ cannot be split into two positive squares */

    /* --- factorization of n (n ≤ 10000) --------------------------- */
    uint64_t original_n = (uint64_t)n;
    uint64_t temp = original_n;
    int two_exp = 0;
    while ((temp & 1ULL) == 0) {    /* count factor 2 */
        temp >>= 1;
        two_exp++;
    }

    seq_t r2 = 4;          /* start with the factor 4 in the formula */

    /* iterate over odd primes up to sqrt(temp) */
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
            }
            if ((p % 4) == 3) {
                /* representation impossible if exponent is odd */
                if (exp & 1) return 0;
            } else { /* p % 4 == 1 */
                r2 *= ((seq_t)5 * (seq_t)exp + 1);
            }
        }
    }
    if (temp > 1) {           /* remaining prime factor */
        uint64_t p = temp;
        int exp = 1;
        if ((p % 4) == 3) {
            if (exp & 1) return 0;
        } else { /* p % 4 == 1 */
            r2 *= ((seq_t)5 * (seq_t)exp + 1);
        }
    }

    /* --- zero‑square representations -------------------------------- */
    seq_t nz = is_square_uint64(original_n) ? (seq_t)4 : (seq_t)0;

    /* --- equal‑square representations (n⁵ = 2·k²) ------------------- */
    seq_t B = 0;
    if ((original_n & 1ULL) == 0)     /* n even */
    {
        bool two_odd = (two_exp & 1) != 0;
        uint64_t odd_part = original_n >> two_exp;
        if (two_odd && is_square_uint64(odd_part))
            B = 1;
    }

    /* --- final count ----------------------------------------------- */
    seq_t C = (r2 - nz) / 4;   /* guaranteed integer */
    seq_t result = (C + B) / 2;
    return result;
}
