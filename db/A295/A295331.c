/*
 * A295331  Numerators of continued fraction convergents to sqrt(13)/2 =
 *          A295330.
 * https://oeis.org/A295331
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 19, u64 n <= 37, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A295331: Numerators of continued fraction convergents to sqrt(13)/2.
   Recurrence: a(0)=1, a(1)=2, a(2)=9, a(3)=128, a(4)=521, a(5)=649, a(6)=1819,
                a(7)=2468, a(8)=11691, a(9)=166142, a(10)=676259, a(11)=842401.
   For n >= 12:  a(n) = 1298 * a(n-6) - a(n-12).  */
/* The function works for any build that provides seq_t and idx_t. */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Base values for 0 <= n < 12 */
    static const seq_t base[12] = {
        (seq_t)1,    (seq_t)2,    (seq_t)9,
        (seq_t)128,  (seq_t)521,  (seq_t)649,
        (seq_t)1819, (seq_t)2468, (seq_t)11691,
        (seq_t)166142, (seq_t)676259, (seq_t)842401
    };

    if (n < 12)
        return base[n];

    /* Working buffer large enough to hold the last 13 terms.
       Index i is stored at buf[i % 13]. */
    static seq_t buf[13];
    /* Initialize the buffer with the known base values */
    for (idx_t i = 0; i < 12; ++i)
        buf[i] = base[i];

    /* Compute terms from 12 up to n */
    for (idx_t i = 12; i <= n; ++i) {
        /* Use the circular buffer to fetch a(i-6) and a(i-12) */
        seq_t term = (seq_t)(1298) * buf[(i - 6) % 13] - buf[(i - 12) % 13];
        buf[i % 13] = term;
    }
    return buf[n % 13];
}
