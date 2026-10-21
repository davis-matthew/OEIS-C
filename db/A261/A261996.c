/*
 * A261996  The first of twenty-one consecutive positive integers the sum of
 *          the squares of which is equal to the sum of the squares of four
 *          consecutive positive integers.
 * https://oeis.org/A261996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 17, u64 n <= 36, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1)+110*a(n-4)-110*a(n-5)-a(n-8)+a(n-9),
   with a(1)=8, a(2)=44, a(3)=128, a(4)=788, a(5)=2024,
   a(6)=5948, a(7)=15176, a(8)=87764, a(9)=223712. */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Ensure n is within the defined range */
    if (n < 1) return (seq_t)0;

    /* Initial nine terms */
    static const seq_t init[9] = {
        (seq_t)8,     (seq_t)44,    (seq_t)128,
        (seq_t)788,   (seq_t)2024,  (seq_t)5948,
        (seq_t)15176, (seq_t)87764, (seq_t)223712
    };

    if (n <= 9) return init[n - 1];

    /* Buffer of the latest nine values */
    seq_t buf[9];
    for (size_t i = 0; i < 9; ++i) buf[i] = init[i];

    seq_t next = 0;
    for (idx_t i = 10; i <= n; ++i) {
        seq_t a1  = buf[8];          /* a(i-1)   */
        seq_t a4  = buf[5];          /* a(i-4)   */
        seq_t a5  = buf[4];          /* a(i-5)   */
        seq_t a8  = buf[1];          /* a(i-8)   */
        seq_t a9  = buf[0];          /* a(i-9)   */

        next = a1
             + (seq_t)110 * a4
             - (seq_t)110 * a5
             - a8
             + a9;

        /* Shift the buffer left by one position */
        for (size_t k = 0; k < 8; ++k) buf[k] = buf[k + 1];
        buf[8] = next;
    }

    return next;
}
