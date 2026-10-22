/*
 * A373407  Smallest positive integer k such that no more than n numbers
 *          (formed by multiplying k by a digit) are anagrams of k, or -1 if
 *          no such number exists.
 * https://oeis.org/A373407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  more, nonn, base, fini
 * Formula:   a(n) = minimal positive integer k such that exactly n numbers (k
 *            multiplied by a digit 1..9) are anagrams of k. The first six
 *            non‑negative values are known: a(1)=1, a(2)=1035, a(3)=123876,
 *            a(4)=1402857, a(5)=1037520684, a(6)=142857. For n>6 no such
 *            number exists (a(n) = -1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* pre‑computed values (index 0 unused) */
    static const seq_t values[10] = {
        (seq_t)(-1),            /* 0 – unused */
        (seq_t)1,               /* 1 */
        (seq_t)1035,            /* 2 */
        (seq_t)123876,          /* 3 */
        (seq_t)1402857,         /* 4 */
        (seq_t)1037520684,      /* 5 */
        (seq_t)142857,          /* 6 */
        (seq_t)(-1),            /* 7 */
        (seq_t)(-1),            /* 8 */
        (seq_t)(-1)             /* 9 */
    };

    if (n < 10) return values[n];
    return (seq_t)(-1);
}
