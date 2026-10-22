/*
 * A373977  a(n) = 1 if n is a nonprime of the form 3*k + 1, otherwise 0.
 * https://oeis.org/A373977
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is a nonprime of the form 3*k + 1, otherwise 0
 * Range:     u8 n <= 8519, u16 n <= 8519, u32 n <= 8519, u64 n <= 8519,
 *            u128 n <= 8519, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* only numbers congruent to 1 mod 3 can possibly be nonprime of that form */
    if (n % (idx_t)3 != (idx_t)1)
        return (seq_t)0;
    /* 1 is counted as a nonprime of the form */
    if (n == (idx_t)1)
        return (seq_t)1;
    /* all other composite numbers ≡1 (mod 3) get 1, primes get 0 */
    return is_prime(n) ? (seq_t)0 : (seq_t)1;
}
