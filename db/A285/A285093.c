/*
 * A285093  Corresponding values of arithmetic means of digits of numbers from
 *          A061383.
 * https://oeis.org/A285093
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3000, u16 n <= 3000, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A285093
   a(0) = 0
   for n ≥ 1, a(n) = A007953(A061383(n)) / A055642(A061383(n))
   i.e. the arithmetic mean of the decimal digits of the n‑th integer (0‑based)
   whose digit mean is an integer. */

/* The implementation enumerates natural numbers, keeps only those whose
   digit sum is divisible by the digit count, and returns the mean for the
   requested index.  All calculations are performed with integer arithmetic
   and fit comfortably in the types seq_t and idx_t. */

#include <stdint.h>
#include <stdlib.h>

static inline unsigned digit_sum_and_count(idx_t v, unsigned *dc)
{
    unsigned sum = 0, c = 0;
    do {
        sum += v % 10;
        v  /= 10;
        ++c;
    } while (v);
    *dc = c;
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    idx_t found = 0;          /* index in the sequence (0‑based) */
    for (idx_t num = 0; ; ++num)
    {
        unsigned cnt;
        unsigned sum = digit_sum_and_count(num, &cnt);
        if (sum % cnt == 0)               /* integer mean -> qualifies */
        {
            if (found == n) return (seq_t)(sum / cnt);
            ++found;
        }
    }
}
