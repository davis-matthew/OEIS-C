/*
 * A326062  a(1) = gcd((sigma(n)-A032742(n))-n, n-A032742(n)), where A032742
 *          gives the largest proper divisor of n.
 * https://oeis.org/A326062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd((sigma(n)-A032742(n))-n, n-A032742(n)) where
 *            A032742(n) = largest proper divisor of n. The function
 *            implements the formula directly using trial division for the
 *            smallest prime factor (hence the largest proper divisor) and
 *            divisor enumeration for sigma(n). GCD is computed via the
 *            Euclid...
 * Range:     u8 n <= 255, u16 n <= 9784, u32 n <= 9784, u64 n <= 9784,
 *            u128 n <= 9784, big any n
 * Helpers:   compute_lpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/compute_lpf.h"

/* Return sigma(n) (sum of all divisors of n) via divisor enumeration.
   Works for any n up to 2^64 - 1. */
static seq_t compute_sigma(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            sum += (seq_t)i;
            idx_t j = n / i;
            if (j != i) sum += (seq_t)j;
        }
    }
    return sum;
}

/* Euclidean algorithm for unsigned seq_t.  Works for arbitrary‑precision
   seq_t types as long as operators +, -, *, /, % are defined. */
static seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 1) return 1; /* defined explicitly */

    seq_t sn = (seq_t)n;
    idx_t spf = compute_lpf(n);
    seq_t d = sn / (seq_t)spf;      /* largest proper divisor */

    seq_t sigma = compute_sigma(n);
    seq_t expr1 = sigma - d - sn;   /* (sigma - d) - n */
    seq_t expr2 = sn - d;           /* n - d */

    return gcd_seq(expr1, expr2);
}
