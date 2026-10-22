/*
 * A303612  a(n) = min{denominator(r) with r in R} and R = {0 <= r rational <=
 *          1 and [r*k] = n}. Here k = 10^(floor(log_10(n))+1) and [x] =
 *          floor(x+1/2) + ceiling((2*x-1)/4) - floor((2*x-1)/4) - 1.
 * https://oeis.org/A303612
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 833, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A303612:  a(n) = minimal number of bits of a denominator
   of a rational r in [0,1] such that bankers round(r*k) = n, where
   k = 10^(digits(n)).
   Implementation uses an incremental search for the smallest
   denominator d that yields a fraction a/d in the required
   interval. No external libraries except standard C headers
   are used. */

#include <stdint.h>
#include <stdbool.h>

/* number of decimal digits of an unsigned integer */
static inline idx_t digits_of(idx_t v)
{
    if (v == 0) return 1;
    idx_t d = 0;
    while (v > 0) {
        v /= 10;
        d++;
    }
    return d;
}

/* integer power of 10: 10^e, 0 <= e <= 20 */
static inline seq_t pow10_seq(idx_t e)
{
    seq_t p = 1;
    while (e--) p *= 10;
    return p;
}

seq_t a(idx_t n)
{
    /* Handle the special case n == 0 (digits = 1, k = 10) */
    if (n == 0) return (seq_t)1;

    /* Number of decimal digits of n and the scaling factor k = 10^dN */
    idx_t dN = digits_of(n);
    seq_t k = pow10_seq(dN);          /* k = 10^(digits(n)) */
    seq_t denom2k = k << 1;           /* 2*k */

    /* Bounds expressed with common denominator 2k */
    seq_t numlow  = (seq_t)2 * n - 1;  /* numerator of lower bound (2n-1)/(2k) */
    seq_t numhigh = (seq_t)2 * n + 1;  /* numerator of upper bound (2n+1)/(2k) */

    /* Incrementally search for the smallest denominator d */
    for (idx_t d = 1; ; ++d) {
        seq_t dd = (seq_t)d;

        seq_t prod_low  = numlow  * dd;   /* (2n-1)*d         */
        seq_t prod_high = numhigh * dd;   /* (2n+1)*d         */

        seq_t lower, upper;
        if ((n & 1) == 0) {              /* n is even: bounds inclusive */
            lower = (prod_low + denom2k - 1) / denom2k; /* ceil(prod_low/2k)   */
            upper = prod_high / denom2k;                 /* floor(prod_high/2k) */
        } else {                                   /* n is odd: bounds exclusive */
            lower = prod_low / denom2k + 1;              /* strictly > lower */
            upper = (prod_high - 1) / denom2k;           /* strictly < upper */
        }

        /* Ensure the selected a/d is not greater than 1 */
        if (upper > dd) upper = dd;

        if (lower <= upper) {
            /* a/d exists with denominator d */
            return (seq_t)d;
        }
        /* otherwise try next denominator */
    }
}
