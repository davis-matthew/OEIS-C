/*
 * A379211  List of positive integers that are congruent to {2, 7, 8, 13} mod
 *          15.
 * https://oeis.org/A379211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15 * floor((n-1)/4) + base[(n-1)%4] where base = {2, 7,
 *            8, 13}. (Offset 1) Recurrence: a(1)=2, a(2)=7, a(3)=8, a(4)=13,
 *            a(n)=a(n-4)+15 for n>4
 * Range:     u8 n <= 68, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* offset: sequence starts at index 1 */
    if (n == (idx_t)0)
        return (seq_t)0;          /* value for n=0 is undefined, return 0 */

    /* k = n-1, q = floor(k/4), r = k%4 */
    idx_t k = n - (idx_t)1;
    idx_t q = k / (idx_t)4;
    idx_t r = k % (idx_t)4;

    /* base terms for remainders 0..3 */
    static const seq_t base[4] = { (seq_t)2, (seq_t)7, (seq_t)8, (seq_t)13 };

    /* compute value: base[r] + 15*q */
    idx_t val = (idx_t)base[(size_t)r] + (idx_t)15 * q;

    return (seq_t)val;
}
