/*
 * A301729  a(0)=1; thereafter positive numbers that are congruent to {0, 1,
 *          3, 5} mod 6.
 * https://oeis.org/A301729
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n==0; otherwise a(n) = 6*floor((n-1)/4) + {1,3,5,6}[
 *            (n-1)%4 ]
 * Range:     u8 n <= 170, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;
    idx_t k = n - 1u;
    idx_t b = k / 4u;
    idx_t r = k % 4u;
    seq_t base = (seq_t)6 * ((seq_t)b);
    switch (r) {
        case 0u: return base + (seq_t)1;
        case 1u: return base + (seq_t)3;
        case 2u: return base + (seq_t)5;
        default: /* r == 3 */ return base + (seq_t)6;
    }
}
