/*
 * A324183  a(n) = d(A163511(n)), where d(n) is A000005, the number of
 *          divisors of n.
 * https://oeis.org/A324183
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000005(A163511(n)); with 1-bit positions q_1<...<q_r of
 *            n (q_0=0), a(n) = Product_{i=1..r} (q_i - q_{i-1} - 1 + [i==r] +
 *            1), a(0)=1
 * Range:     u8 n <= 17553, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 122 terms through n = 9030 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t result = 1;
    idx_t prev = 0;
    idx_t bitpos = 1;
    idx_t tmp = n;
    while (tmp) {
        if (tmp & 1) {
            idx_t e = bitpos - prev - 1;
            prev = bitpos;
            if ((tmp >> 1) == 0) e += 1; /* extra factor p(r) at the last set bit */
            result *= (seq_t)(e + 1);
        }
        ++bitpos;
        tmp >>= 1;
    }
    return result;
}
