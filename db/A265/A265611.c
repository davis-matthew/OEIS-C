/*
 * A265611  a(n) = a(n-1) + floor((n-1)/2) - (-1)^n + 2 for n>=2, a(0)=1,
 *          a(1)=3.
 * https://oeis.org/A265611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(0)=1, a(2k)=k*(k+3), a(2k+1)=(k+1)*(k+3)
 * Range:     u8 n <= 29, u16 n <= 509, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if ((n & 1) == 0) {                    /* even n : n = 2k */
        seq_t k = (seq_t)(n >> 1);
        return k * (k + 3);
    } else {                               /* odd n  : n = 2k+1 */
        seq_t k = (seq_t)((n - 1) >> 1);
        return (k + 1) * (k + 3);
    }
}
