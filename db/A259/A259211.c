/*
 * A259211  Positive octagonal numbers (A000567) that are other octagonal
 *          numbers divided by 3.
 * https://oeis.org/A259211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7300803*a(n-1)-7300803*a(n-2)+a(n-3) a(1)=560,
 *            a(2)=4090071480, a(3)=29860802042948800
 * Range:     u8 none, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)560;
    if (n == 2) return (seq_t)4090071480;
    if (n == 3) return (seq_t)29860802042948800;

    seq_t a1 = (seq_t)560;
    seq_t a2 = (seq_t)4090071480;
    seq_t a3 = (seq_t)29860802042948800;
    seq_t an;

    for (idx_t i = (idx_t)4; i <= n; ++i) {
        an = (seq_t)7300803 * a3 - (seq_t)7300803 * a2 + a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return a3;
}
