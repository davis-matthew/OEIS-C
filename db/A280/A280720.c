/*
 * A280720  For p = prime(n), number of iterations of the function f(x) = 5x +
 *          2 that leave p prime.
 * https://oeis.org/A280720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of successive primes produced by iterating
 *            f(x)=5x+2 starting from the nth prime prime(n)
 * Range:     u8 n <= 5, u16 n <= 73, u32 n <= 73, u64 n <= 8276,
 *            u128 n <= 8276, big any n
 * Helpers:   get_prime, is_prime_big, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/get_prime.h"
#include "../utils/mul_small.h"
#include "../utils/is_prime_big.h"

/* External function: seq_t a(idx_t n) */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    seq_t x   = (seq_t)get_prime(n);   /* x_0 = p_n */
    seq_t cnt = (seq_t)0;

    while (1) {
        x = mul_small(x, 5);           /* x = 5*x */
        x = x + (seq_t)2;              /* x = x + 2   */
        if (is_prime_big(x)) {
            cnt = cnt + (seq_t)1;      /* prime, count */
        } else {
            break;                    /* composite, stop */
        }
    }
    return cnt;
}
