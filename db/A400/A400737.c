/*
 * A400737  Least number k such that the sum of the numbers from k to k + n -
 *          1 is divisible by the sum of their digits.
 * https://oeis.org/A400737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, new
 * Formula:   a(n) = least k such that the sum of numbers k + (k+1) + … +
 *            (k+n-1) is divisible by the sum of the decimal digits of all
 *            those numbers
 * Range:     u8 n <= 9, u16 n <= 60, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;          /* cast only once */
    seq_t k = (seq_t)1;          /* starting value of k */

    /* Initial sum of the arithmetic sequence */
    seq_t sum_numbers = N * (((seq_t)2 * k) + N - (seq_t)1) / (seq_t)2;

    /* Initial sum of digits of the numbers k .. k+n-1 */
    seq_t sum_digits = (seq_t)0;
    for (idx_t i = (idx_t)0; i < n; ++i)
        sum_digits += digit_sum(k + (seq_t)i);

    /* Search for the minimal k */
    for (;;)
    {
        if ((sum_numbers % sum_digits) == (seq_t)0)
            return k;                   /* found the required k */

        /* Move to the next integer k+1 */
        sum_numbers += N;                   /* each term increases by 1 */
        sum_digits -= digit_sum(k);          /* remove k's digit sum */
        sum_digits += digit_sum(k + N);      /* add (k+n)'s digit sum */
        ++k;
    }
}
