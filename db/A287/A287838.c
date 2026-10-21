/*
 * A287838  Number of words of length n over the alphabet {0,1,...,10} such
 *          that no two consecutive terms have distance 8.
 * https://oeis.org/A287838
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) + 5*a(n-2) for n > 2, a(0)=1, a(1)=11, a(2)=115
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)11;
    if (n == 2) return (seq_t)115;

    /* We need a(1) and a(2) to start the recurrence for n >= 3 */
    seq_t prev2 = (seq_t)11;  /* a(n-2) */
    seq_t prev1 = (seq_t)115; /* a(n-1) */
    seq_t curr;               /* a(n) */

    for (idx_t i = 3; i <= n; ++i) {
        curr  = (seq_t)10 * prev1 + (seq_t)5 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
