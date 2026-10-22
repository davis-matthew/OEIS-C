/*
 * A363955  When the base-2 representation of n is interpreted as a Gaussian
 *          integer x+yi in base (-1+i), both x and y are nonnegative.
 * https://oeis.org/A363955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n-th non‑negative integer whose binary representation,
   interpreted as a Gaussian integer in base (-1+i), has non‑negative
   real and imaginary parts. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Pre‑computed values for (-1+i)^k.                          */
/* re[k] + i*im[k] = (-1+i)^k (k = 0 .. 64).                  */
static bool powers_inited = false;
static int64_t re[65];
static int64_t im[65];

static void init_powers(void)
{
    re[0] = 1;
    im[0] = 0;
    for (int k = 1; k <= 64; ++k) {
        int64_t pr = re[k - 1];
        int64_t pi = im[k - 1];
        /* Multiply (pr + pi*i) by (-1+i): */
        re[k] = -pr - pi;      /* real part  = (-1)*pr +     (-1)*pi  */
        im[k] =  pr - pi;      /* imag part  = (-1)*pr +      1 *pi   */
    }
    powers_inited = true;
}

/* Test whether a binary number n satisfies the condition. */
static bool is_member(idx_t n)
{
    int64_t r = 0;
    int64_t i = 0;
    int pos = 0;
    while (n) {
        if (n & 1) {
            r += re[pos];
            i += im[pos];
        }
        n >>= 1;
        ++pos;
    }
    return (r >= 0) && (i >= 0);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) {            /* For completeness: a(0) is undefined.    */
        return (seq_t)0;
    }
    if (!powers_inited) {
        init_powers();
    }

    idx_t count = 0;
    idx_t candidate = 0;
    while (count < n) {
        if (is_member(candidate)) {
            ++count;
            if (count == n) {
                return (seq_t)candidate;
            }
        }
        ++candidate;
    }
    /* Unreachable but keeps compiler happy. */
    return (seq_t)0;
}
