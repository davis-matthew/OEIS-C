/*
 * A280448  Sum of the GCDs of the smaller and larger parts of the partitions
 *          of 2n into two squarefree parts.
 * https://oeis.org/A280448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..n} gcd(i,2n-i)*mu(i)^2*mu(2n-i)^2
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   gcd, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (mu(m)^2 is 1 iff m is squarefree, 0 otherwise.) */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* undefined for n==0, return 0 */

    idx_t two_n = n + n;                 /* 2n as an idx_t */
    seq_t sum = 0;

    for (idx_t i = 1; i <= n; ++i) {
        idx_t j = two_n - i;             /* i + j = 2n */

        if (is_squarefree((seq_t)i) && is_squarefree((seq_t)j)) {
            seq_t g = gcd((seq_t)i, (seq_t)j);
            sum += g;
        }
    }

    return sum;
}
