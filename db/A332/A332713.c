/*
 * A332713  a(n) = Sum_{d|n} phi(d/gcd(d, n/d)).
 * https://oeis.org/A332713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = Sum_{d|n} phi(d/gcd(d, n/d))
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/phi.h"

/* compute phi(d / gcd(d, n/d)) */
static inline seq_t phi_of_divisor(idx_t d, idx_t n)
{
    idx_t nd = n / d;                         /* n/d */
    seq_t g = gcd((seq_t)d, (seq_t)nd);        /* gcd(d, n/d) */
    idx_t gidx = (idx_t)g;                     /* safe back to idx_t */
    idx_t div = d / gidx;                      /* d/g */
    return phi(div);                           /* phi(div) */
}

/* main sequence function */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= n / i; ++i)          /* enumerate divisors up to sqrt(n) */
    {
        if (n % i == 0)
        {
            sum += phi_of_divisor(i, n);         /* divisor i */
            idx_t d2 = n / i;
            if (d2 != i)
                sum += phi_of_divisor(d2, n);    /* paired divisor */
        }
    }
    return sum;
}
