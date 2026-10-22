/*
 * A319280  Numbers that are congruent to {0, 4, 7, 11} mod 12.
 * https://oeis.org/A319280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-4) + 12, with a(1)=0, a(2)=4, a(3)=7, a(4)=11
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t base[4] = {0,4,7,11};
    if (n <= 4) return base[n-1];
    const idx_t k = (n - 1) / 4;
    const unsigned r = (unsigned)((n - 1) % 4);
    return base[r] + ((seq_t)12) * ((seq_t)k);
}
