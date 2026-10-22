/*
 * A399858  a(n) is the book Ramsey number R(B_n,B_n).
 * https://oeis.org/A399858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   a(n) = 4*n + 1 if 4*n + 1 is squarefree and composite, a(n) =
 *            4*n + 2 otherwise (i.e., if 4*n + 1 is prime or contains a
 *            squared factor)
 * Range:     u8 n <= 13, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* m = 4*n + 1 (the argument to check) */
    idx_t m_idx = 4 * n + 1;
    seq_t m = (seq_t)m_idx;

    /* Check whether 4*n + 1 is prime */
    bool prime = is_prime(m_idx);

    /* Check whether 4*n + 1 is squarefree:
       it is not squarefree if it contains a prime factor whose exponent > 1.
    */
    bool squarefree = true;
    idx_t tmp = m_idx;
    for (idx_t i = 2; i * i <= tmp; ++i) {
        if (tmp % i == 0) {
            int exp = 0;
            while (tmp % i == 0) {
                tmp /= i;
                ++exp;
            }
            if (exp > 1) {
                squarefree = false;
                break;
            }
        }
    }

    /* Final value */
    if (prime || !squarefree) {
        return m + 1;
    } else {
        return m;
    }
}
