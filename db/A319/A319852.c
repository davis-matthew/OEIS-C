/*
 * A319852  Difference between 3^n and the product of primes less than or
 *          equal to n.
 * https://oeis.org/A319852
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = 3^n - n# (product of primes ≤ n)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   is_prime, pow_uint, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"
#include "../utils/seq_mul.h"
#include "../utils/pow_uint.h"

/* External linkage function as required by the build system */
seq_t a(idx_t n)
{
    /* 3^n */
    seq_t pow = pow_uint(3, n);

    /* n# = product of primes ≤ n */
    seq_t prod = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i) {
        if (is_prime(i)) {
            prod = seq_mul(prod, (seq_t)i);
        }
    }

    return pow - prod;
}
