/*
 * A353483  a(n) = 1 if n is a squarefree semiprime of the form (4t+3)*(4u+3),
 *          otherwise 0.
 * https://oeis.org/A353483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is a squarefree semiprime of the form
 *            (4t+3)*(4u+3), * otherwise 0
 * Range:     u8 n <= 9944, u16 n <= 9944, u32 n <= 9944, u64 n <= 9944,
 *            u128 n <= 9944, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t x = (uint64_t)n;
    uint64_t temp = x;
    int cnt = 0;

    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            if (p % 4 != 3)        /* prime factor not ≡3 (mod 4) */
                return 0;
            int exp = 0;
            while (temp % p == 0) { /* count exponent of p */
                temp /= p;
                ++exp;
                if (exp > 1)       /* not square‑free */
                    return 0;
            }
            ++cnt;
            if (cnt > 2)           /* more than two prime factors */
                return 0;
        }
    }

    if (temp > 1) {                 /* remaining prime factor */
        if (temp % 4 != 3)
            return 0;
        ++cnt;
    }

    return cnt == 2 ? 1 : 0;
}
