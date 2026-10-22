/*
 * A396423  a(n) = r(n,n), where r(1,k)=f(k)=floor(k*c), r(n,k)=f(r(n-1,k)),
 *          c=5/3.
 * https://oeis.org/A396423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = r(n,n) where c = 5/3, r(1,k) = floor(k * c), r(n,k) =
 *            floor(r(n‑1,k) * c). The function returns a(n)
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for offset 1, but harmless */
        return (seq_t)0;

    seq_t x = (seq_t)n;
    idx_t i;
    for (i = 0; i < n; ++i) {
        x = (x * 5) / 3;   /* floor(x * 5/3) using integer division */
    }
    return x;
}
