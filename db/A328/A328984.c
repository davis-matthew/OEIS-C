/*
 * A328984  If n is even, a(n) = floor((5t+1)/2) where t=n/2; if n == 1 (mod
 *          4) then a(n) = 10t+1 where t = (n-1)/4; and if n == 3 (mod 4) then
 *          a(n) = 10t+7 where t = (n-3)/4.
 * https://oeis.org/A328984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   If n is even, a(n) = floor((5t+1)/2) where t = n/2; If n ≡ 1
 *            (mod 4), a(n) = 10t+1 where t = (n-1)/4; If n ≡ 3 (mod 4), a(n)
 *            = 10t+7 where t = (n-3)/4;
 * Range:     u8 n <= 102, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1-indexed, n ≥ 1 */
    if ((n & 1UL) == 0) {
        /* even n */
        seq_t t = (seq_t)(n >> 1);
        return (5 * t + 1) / 2;
    } else if ((n & 3UL) == 1) {
        /* n ≡ 1 (mod 4) */
        seq_t t = (seq_t)((n - 1) / 4);
        return 10 * t + 1;
    } else {
        /* n ≡ 3 (mod 4) */
        seq_t t = (seq_t)((n - 3) / 4);
        return 10 * t + 7;
    }
}
