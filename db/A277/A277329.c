/*
 * A277329  a(0)=0, for n >= 1, a(2n) = a(n)+1, a(4n-1) = a(n)+1, a(4n+1) =
 *          a(n)+1.
 * https://oeis.org/A277329
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0; for n>=1: if n is even: a(n) = a(n/2) + 1 if n≡1 (mod
 *            4): a(n) = a((n-1)/4) + 1 if n≡3 (mod 4): a(n) = a((n+1)/4) + 1
 *            This computes the number of recursive reductions needed to reach
 *            0
 * Range:     u8 n <= 120, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t v = 0;
    while (n > 0) {
        if ((n & (idx_t)1) == (idx_t)0) {
            n >>= 1;          /* even: n = n/2         */
        } else if ((n & (idx_t)3) == (idx_t)1) {
            n = (n - (idx_t)1) >> 2;  /* n≡1 (mod 4): (n-1)/4 */
        } else {
            n = (n + (idx_t)1) >> 2;  /* n≡3 (mod 4): (n+1)/4 */
        }
        ++v;
    }
    return v;
}
