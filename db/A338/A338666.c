/*
 * A338666  a(1)=1 and a(2)=2. For all n > 2, a(n) is the smallest number >
 *          a(n-1) by a number > the difference between a(n-1) and a(n-2) so
 *          that consecutive terms of sequence are always relatively prime.
 * https://oeis.org/A338666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 277, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A338666:
   a(1)=1, a(2)=2.
   For n>2, a(n) is the smallest integer > a(n-1) such that
   a(n)-a(n-1) > a(n-1)-a(n-2) and gcd(a(n-1),a(n)) = 1.   */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    seq_t prev2 = (seq_t)1;   /* a(n-2)   */
    seq_t prev  = (seq_t)2;   /* a(n-1)   */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t diff = prev - prev2;                     /* a(n-1)-a(n-2)   */
        seq_t cand = prev + diff + (seq_t)1;           /* start candidate */
        while (gcd(prev, cand) != (seq_t)1)            /* ensure coprime   */
            cand++;
        prev2 = prev;                                  /* shift window    */
        prev  = cand;
    }
    return prev;
}
