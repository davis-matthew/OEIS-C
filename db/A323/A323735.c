/*
 * A323735  a(n) is the largest minimal distance of a binary LCD [n,2] code.
 * https://oeis.org/A323735
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4 * floor(n / 6) + offset where offset = -1 for n % 6 ==
 *            0, 0 for n % 6 == 1, 1 for n % 6 == 2, 2 for n % 6 == 3,4,5
 *            (This closed form is equivalent to the recurrence a(n) = a(n‑6)
 *            + 4 for n > 7 with the initial values
 *            a(2)=1,a(3)=2,a(4)=2,a(5)=2,a(6)=3,a(7)=4.)
 * Range:     u8 n <= 384, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 2 in the test harness. The
       function is independent of any earlier calls, so no
       memoisation is required.                                    */
    idx_t k = n / 6;            /* floor(n/6) */
    idx_t r = n % 6;            /* n modulo 6 */

    seq_t base = (seq_t)k * 4;  /* 4 * floor(n/6) */

    switch (r) {
        case 0:  return base - 1;        /* n % 6 == 0 */
        case 1:  return base;            /* n % 6 == 1 */
        case 2:  return base + 1;        /* n % 6 == 2 */
        default: return base + 2;        /* n % 6 == 3,4,5 */
    }
}
