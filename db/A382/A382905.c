/*
 * A382905  The powerfree part of the n-th biquadratefree number.
 * https://oeis.org/A382905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A382905 – powerfree part of the n‑th biquadratefree number.
   a(n) = powerfree_part(b(n)),
   where b(n) is the nth integer not divisible by any prime^4. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/floor_sqrt.h"   /* floor_sqrt(seq_t) */

static const seq_t PRIME_POW4[] = {
    (seq_t)16,     /* 2⁴ */
    (seq_t)81,     /* 3⁴ */
    (seq_t)625,    /* 5⁴ */
    (seq_t)2401,   /* 7⁴ */
    (seq_t)14641,  /* 11⁴ */
    (seq_t)28561,  /* 13⁴ */
    (seq_t)83521   /* 17⁴ */
};
static const size_t PRIME_POW4_CNT = sizeof(PRIME_POW4) / sizeof(*PRIME_POW4);

/* Return the product of primes that divide x exactly once. */
static seq_t powerfree_part(seq_t x)
{
    seq_t result = 1;
    seq_t temp   = x;
    seq_t limit  = floor_sqrt(temp);

    for (seq_t p = 2; p <= limit; ++p) {
        if (temp % p != 0)
            continue;
        int exp = 0;
        while (temp % p == 0) {
            temp /= p;
            ++exp;
        }
        if (exp == 1)
            result *= p;
        limit = floor_sqrt(temp);
    }

    if (temp > 1)          /* remaining prime factor */
        result *= temp;

    return result;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;         /* offset is 1, this value is unused */

    idx_t  found = 0;
    seq_t  num   = 0;

    for (;;) {
        ++num;
        bool bad = false;
        for (size_t i = 0; i < PRIME_POW4_CNT; ++i) {
            if (num % PRIME_POW4[i] == 0) {
                bad = true;
                break;
            }
        }
        if (bad)
            continue;

        ++found;
        if (found == n)
            return powerfree_part(num);
    }
}
