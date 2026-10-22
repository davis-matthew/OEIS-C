/*
 * A338668  a(n) is the rightmost prime number in prime tower factorization of
 *          n; a(1) = 1.
 * https://oeis.org/A338668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the rightmost prime in prime tower factorization of n */
#include <stdint.h>

static seq_t a_rec(idx_t n)
{
    idx_t largest = 0;
    idx_t exp = 0;
    idx_t temp = n;

    if (temp % 2 == 0) {
        largest = 2;
        exp = 0;
        while (temp % 2 == 0) {
            temp /= 2;
            exp++;
        }
    }
    for (idx_t i = 3; i * i <= temp; i += 2) {
        if (temp % i == 0) {
            largest = i;
            exp = 0;
            while (temp % i == 0) {
                temp /= i;
                exp++;
            }
        }
    }
    if (temp > 1) {
        largest = temp;
        exp = 1;
    }

    if (exp == 1) {
        return (seq_t)largest;
    } else {
        return a_rec(exp);
    }
}

seq_t a(idx_t n)
{
    if (n <= 1) {
        return 1;
    }
    return a_rec(n);
}
