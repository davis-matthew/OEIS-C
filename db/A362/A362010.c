/*
 * A362010  Numbers k such that 1 < gcd(k, 42) < k and A007947(k) does not
 *          divide 42.
 * https://oeis.org/A362010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 138, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A362010: numbers k such that
 *   1 < gcd(k, 42) < k   and   rad(k) does NOT divide 42.
 * The function searches for the nth such number by scanning integers
 * starting from 2.  It works for any idx_t index n >= 1 and any
 * seq_t value type supported by the build. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static const seq_t _V42 = (seq_t)42;   /* 42 as a seq_t */

seq_t a(idx_t n)
{
    if (n == 0)            /* O.EIS uses offset 1, behaviour for n==0 not needed */
        return (seq_t)0;

    idx_t   k      = 2;    /* current integer to test */
    idx_t   found  = 0;    /* how many qualifying numbers found so far */

    while (1) {
        /* compute gcd(k,42) using the provided routine */
        seq_t g = gcd((seq_t)k, _V42);

        /* condition 1: 1 < gcd(k,42) < k   and   k is not a divisor of 42 */
        if (g > (seq_t)1 && g < (seq_t)k) {
            /* compute rad(k) (product of distinct prime factors)  */
            uint64_t tmp = k;
            uint64_t rad = 1;
            for (uint64_t p = 2; p * p <= tmp; ++p) {
                if (tmp % p == 0) {
                    rad *= p;
                    while (tmp % p == 0) tmp /= p;
                }
            }
            if (tmp > 1) rad *= tmp;

            /* condition 2: rad(k) does not divide 42 */
            if (42 % rad != 0) {
                ++found;
                if (found == n)       /* nth qualifying number found */
                    return (seq_t)k;
            }
        }
        ++k;
    }
}
