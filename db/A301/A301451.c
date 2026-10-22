/*
 * A301451  Numbers congruent to {1, 7} mod 9.
 * https://oeis.org/A301451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*floor((n-1)/2) + (n%2?1:7) (for n ≥ 1)
 * Range:     u8 n <= 57, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* safety: offset starts at 1 */
    seq_t k   = ((seq_t)(n - 1)) / 2;     /* floor((n-1)/2) */
    seq_t odd = (seq_t)(n & 1);           /* 1 if n is odd, 0 if even */
    return k * 9 + (odd ? 1 : 7);
}
