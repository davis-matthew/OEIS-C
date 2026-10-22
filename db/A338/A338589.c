/*
 * A338589  Sum of the remainders (s*t mod n), where s + t = n and 1 <= s <=
 *          t.
 * https://oeis.org/A338589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = Sum_{i=1..floor(n/2)} ( i*(n-i) mod n )
 * Range:     u8 n <= 29, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;      /* convert index to seq_t */
    seq_t sum = 0;
    idx_t limit = n / 2;     /* floor(n/2) */

    for (idx_t i = 1; i <= limit; ++i) {
        seq_t i_t        = (seq_t)i;          /* cast loop variable */
        seq_t prod       = i_t * (N - i_t);   /* i*(n-i) */
        sum += prod % N;                     /* add remainder */
    }

    return sum;
}
