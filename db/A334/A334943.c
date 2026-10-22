/*
 * A334943  a(1) = 1, a(n) = a(n-1) / gcd(a(n-1),n) if this gcd is > 1, else
 *          a(n) = 3*a(n-1) + n + 1.
 * https://oeis.org/A334943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(n) = a(n-1)/gcd(a(n-1),n) if gcd>1 else 3*a(n-1)+n+1
 * Range:     u8 n <= 31, u16 n <= 571, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/gcd.h"

/* Compute the n-th term of sequence A334943.
 * The function works for any integer type seq_t provided by the build
 * and uses only integer arithmetic together with the gcd routine. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* undefined for indices below the offset */

    seq_t val = 1;  /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t g = gcd(val, (seq_t)i);
        if (g > (seq_t)1)
            val = val / g;
        else
            val = 3 * val + (seq_t)i + (seq_t)1;
    }
    return val;
}
