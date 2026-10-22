/*
 * A300228  a(n) = number of steps in simple Euclidean algorithm for gcd(n,k)
 *          to reach the termination test n=k when starting with n = n and k =
 *          1+phi(n).
 * https://oeis.org/A300228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of steps in the simple Euclidean subtraction
 *            algorithm * for gcd(n, k) where k = 1 + φ(n), starting from the
 *            pair (n, k) and * terminating when the two numbers become equal
 * Range:     u8 n <= 343, u16 n <= 9844, u32 n <= 9844, u64 n <= 9844,
 *            u128 n <= 9844, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute Euler's totient φ(n) for n ≥ 1.  φ(1) = 1. */
static seq_t phi_of(idx_t n)
{
    if (n <= 1) return 1;   /* φ(1) = 1 by convention */
    seq_t result = n;                 /* work in seq_t to avoid overflow */
    idx_t temp = n;

    for (idx_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            while (temp % p == 0) temp /= p;
            result -= result / p;
        }
    }
    if (temp > 1) result -= result / temp;
    return result;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Initial pair (x, y) = (n, 1 + φ(n)) */
    seq_t x = n;
    seq_t y = phi_of(n) + 1;

    seq_t steps = 0;

    /* Simple subtraction Euclidean algorithm until x == y */
    while (x != y)
    {
        if (x > y)
            x -= y;
        else
            y -= x;
        ++steps;
    }
    return steps;
}
