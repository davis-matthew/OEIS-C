/*
 * A245624  Sequence of distinct least positive numbers such that the average
 *          of the first n terms is a cube.
 * https://oeis.org/A245624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2k-1) = k^3, a(2k) = 7*k^3 + 6*k^2 + 2*k
 * Range:     u8 n <= 7, u16 n <= 41, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                  /* undefined for n<1, harmless fallback */
    idx_t k = (n + 1) / 2;          /* k = ceil(n/2) */
    seq_t kk = (seq_t)k;
    if ((n & 1) == 1) {            /* odd n → a(2k-1) = k³ */
        seq_t k2 = kk * kk;
        return kk * k2;
    } else {                       /* even n → a(2k) = 7k³ + 6k² + 2k */
        seq_t k2 = kk * kk;
        seq_t k3 = k2 * kk;
        return (seq_t)7 * k3 + (seq_t)6 * k2 + (seq_t)2 * kk;
    }
}
