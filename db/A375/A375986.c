/*
 * A375986  Maximum number of edges in a simple polygon that is the regular
 *          union of n triangles.
 * https://oeis.org/A375986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more, changed
 * Formula:   a(n) = 11*n - 14 + 2*floor((n+1)/3) for n>=5; a(1)=3, a(2)=12,
 *            a(3)=22, a(4)=33
 * Range:     u8 n <= 23, u16 n <= 25, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    switch (n) {
        case 1: return (seq_t)3;
        case 2: return (seq_t)12;
        case 3: return (seq_t)22;
        case 4: return (seq_t)33;
        default:
            {
                idx_t val = 11 * n - 14 + 2 * ((n + 1) / 3);
                return (seq_t)val;
            }
    }
}
