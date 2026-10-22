/*
 * A322144  a(n) = Sum_{i=1..phi(n)-1} (r(i+1)-r(i))^2 where r(1) = 1 < ... <
 *          n-1 = r(phi(n)) are the phi(n) integers relatively prime to n.
 * https://oeis.org/A322144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1}^{phi(n)-1} (r(i+1)-r(i))^2, where
 *            r(1)=1<…<r(phi(n)) are the integers < n that are coprime to n.
 *            The sequence starts at n=1
 * Range:     u8 n <= 59, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n <= 2) {
        return 0;
    }

    seq_t sum = 0;
    idx_t prev = 1;                      /* r(1) is always 1 when n > 2 */

    for (idx_t i = 2; i < n; ++i) {
        if (gcd_idx(i, n) == 1) {         /* i is a reduced residue */
            idx_t diff = i - prev;
            sum += (seq_t)diff * (seq_t)diff;
            prev = i;
        }
    }

    return sum;
}
