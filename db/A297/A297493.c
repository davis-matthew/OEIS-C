/*
 * A297493  a(n) = (1/2) * Sum_{|k|<=2*sqrt(p)} k^8*H(4*p-k^2) where H() is
 *          the Hurwitz class number and p is n-th prime.
 * https://oeis.org/A297493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 14*prime(n)^5 - 28*prime(n)^3 - 20*prime(n)^2 -
 *            7*prime(n) - 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 15, u64 n <= 575, u128 n <= 1000,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here prime(n) is the nth prime, 1‑based */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* n ≥ 1: obtain the nth prime */
    uint32_t p = get_prime(n);
    seq_t x = (seq_t)p;

    /* Compute powers of x in seq_t */
    seq_t x2 = x * x;       /* x^2 */
    seq_t x3 = x2 * x;      /* x^3 */
    seq_t x5 = x3 * x2;     /* x^5 = x^3 * x^2 */

    /* Polynomial b(x) = 14*x^5 - 28*x^3 - 20*x^2 - 7*x - 1 */
    seq_t res = (seq_t)14 * x5;
    res -= (seq_t)28 * x3;
    res -= (seq_t)20 * x2;
    res -= (seq_t)7 * x;
    res -= (seq_t)1;

    return res;
}
