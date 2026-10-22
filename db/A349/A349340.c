/*
 * A349340  Dirichlet inverse of A003557, where A003557 is multiplicative with
 *          a(p^e) = p^(e-1).
 * https://oeis.org/A349340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   not stated
 * Range:     i8 n <= 2196, i16 n <= 9826, i32 n <= 9826, i64 n <= 9826,
 *            i128 n <= 9826, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A349340: Dirichlet inverse of A003557
   Multiplicative with a(p^e) = -((p-1)^(e-1)).
   a(1) = 1, a(n) = product over prime powers of n of the above. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    seq_t result = 1;
    idx_t temp = n;

    /* factor 2 separately */
    int exp2 = 0;
    while ((temp & 1ULL) == 0) {
        temp >>= 1;
        ++exp2;
    }
    if (exp2 > 0) {
        seq_t term = -big_pow((seq_t)(2 - 1), (seq_t)(exp2 - 1));
        result = (result * term);
    }

    /* odd factors */
    idx_t d = 3;
    while (d * d <= temp) {
        if (temp % d == 0) {
            int e = 0;
            while (temp % d == 0) {
                temp /= d;
                ++e;
            }
            seq_t term = -big_pow((seq_t)(d - 1), (seq_t)(e - 1));
            result = (result * term);
        }
        d += 2;
    }

    /* remaining prime factor > 1 */
    if (temp > 1) {
        seq_t term = -big_pow((seq_t)(temp - 1), (seq_t)0);
        result = (result * term);
    }
    return result;
}
