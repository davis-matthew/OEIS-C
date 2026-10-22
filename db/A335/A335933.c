/*
 * A335933  A fractal function, related to ruler functions. a(1) = 0;
 *          otherwise for m >= 0, a(3m) = 1, a(3m-1) = a(2m-1) +
 *          sign(a(2m-1)), a(3m+1) = a(2m+1) + sign(a(2m+1)).
 * https://oeis.org/A335933
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=0; a(n) = 1 if n%3==0; otherwise a(n) =
 *            a(2*(n/3)+1) + (a(2*(n/3)+1)>0)
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n % 3 == 0) return (seq_t)1;
    /* m = 2*(n/3)+1  fits in idx_t because 2*(n/3)+1 <= 2*n/3+1 < 2^64 for 0<=n<2^64 */
    idx_t m = 2 * (n / 3) + 1;
    seq_t val = a(m);
    return val + (val > (seq_t)0 ? (seq_t)1 : (seq_t)0);
}
