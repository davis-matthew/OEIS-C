/*
 * A288677  Every element of Z/nZ can be expressed as a sum of no more than
 *          a(n) squares.
 * https://oeis.org/A288677
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n=2; 2 if n≠0 mod 4 and every prime squared divisor
 *            p satisfies p≡1 mod 4; * otherwise 3 if n≠0 mod 8, else 4
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 115 terms through n = 9366 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool squarefree_condition(idx_t n)
{
    /* n is guaranteed to be odd since n % 4 != 0 */
    idx_t temp = n;
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
                if (e >= 2 && p % 4 != 1)
                    return false;
            }
        }
    }
    /* remaining factor > sqrt(original n) has exponent 1, no effect */
    return true;
}

seq_t a(idx_t n)
{
    if (n == 2)
        return (seq_t)1;

    if (n % 4 != 0) {
        if (squarefree_condition(n))
            return (seq_t)2;
    }

    return (n % 8 != 0) ? (seq_t)3 : (seq_t)4;
}
