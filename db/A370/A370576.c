/*
 * A370576  a(n) is the difference between the number of n-dist-increasing and
 *          (n-1)-dist-increasing permutations p of [2n], where p is
 *          k-dist-increasing if k>=0 and p(i)<p(i+k) for all i in [2n-k].
 * https://oeis.org/A370576
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=1, a(2)=5, a(3)=70
   a(n) = (2n-1)*n*a(n-1)   for n >= 4  */
/* Note: works for any seq_t that supports + - * / and has unsigned overflow
   (or arbitrary-precision). */

#include <stdlib.h>   /* for NULL */

seq_t a(idx_t n)
{
    /* Handle the known base cases directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)70;

    /* Compute the value iteratively from a(3) up to a(n). */
    seq_t result = (seq_t)70;          /* a(3) */
    for (idx_t i = 4; i <= n; ++i) {
        result *= (seq_t)i;            /* multiply by n */
        result *= (seq_t)(2 * i - 1);  /* multiply by (2n-1) */
    }
    return result;
}
