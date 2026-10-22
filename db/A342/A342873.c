/*
 * A342873  Numbers whose distance to the nearest cube equals the distance to
 *          the nearest product of 3 consecutive integers (three-dimensional
 *          oblong).
 * https://oeis.org/A342873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=0; for n≥2: - if n is even, let k=n/2 then a(n)=8*k³ – k -
 *            if n is odd, let k=(n−1)/2 then a(n)=8*k³ + 6*k² + 2*k
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 0;
    if (n & 1U) {                      /* n is odd, use second form */
        seq_t k = (seq_t)((n - 1) / 2);
        seq_t k2 = k * k;
        seq_t k3 = k2 * k;
        return 8 * k3 + 6 * k2 + 2 * k;
    } else {                           /* n is even, use first form */
        seq_t k = (seq_t)(n / 2);
        seq_t k2 = k * k;
        seq_t k3 = k2 * k;
        return 8 * k3 - k;
    }
}
