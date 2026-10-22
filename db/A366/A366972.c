/*
 * A366972  a(n) = Sum_{k=4..n} floor(n/k).
 * https://oeis.org/A366972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=4..n} floor(n/k)
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 4)
        return 0;

    /* Compute S(n) = sum_{k=1..n} floor(n/k) using the
       classic O(sqrt(n)) division grouping trick. */
    seq_t total = 0;
    for (idx_t i = 1; i <= n; ) {
        idx_t q    = n / i;      /* floor(n/i)   */
        idx_t last = n / q;      /* largest j with floor(n/j)=q */
        seq_t cnt  = (seq_t)(last - i + 1);
        total += (seq_t)q * cnt;
        i = last + 1;
    }

    /* Subtract the first three terms of the sum to get k=4..n. */
    seq_t sub = (seq_t)n
              + (seq_t)(n / 2)
              + (seq_t)(n / 3);

    return total - sub;
}
