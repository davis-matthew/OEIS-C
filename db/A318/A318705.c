/*
 * A318705  For any n >= 0 with base-9 representation Sum_{k=0..w} d_k * 9^k,
 *          let g(n) = Sum_{k=0..w} s(d_k) * 3^k (where s(0) = 0, s(1+2*j) =
 *          i^j and s(2+2*j) = i^j * (1+i) for any j > 0, and i denotes the
 *          imaginary unit); a(n) is the real part of g(n).
 * https://oeis.org/A318705
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = real part of Σ_{k} s(d_k) * 3^k , where s(0)=0,
 *            s(1+2j)=i^j, s(2+2j)=i^j*(1+i) (j≥0). In particular a(9k)=3·a(k)
 * Range:     i8 n <= 6560, i16 n <= 6560, i32 n <= 6560, i64 n <= 6560,
 *            i128 n <= 6560, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* real parts of s(d) for d=0..8 */
    static const seq_t s_real[9] = {
        0,  /* d=0 */
        1,  /* d=1 */
        1,  /* d=2 */
        0,  /* d=3 */
       -1,  /* d=4 */
       -1,  /* d=5 */
       -1,  /* d=6 */
        0,  /* d=7 */
        1   /* d=8 */
    };

    idx_t  t   = n;
    seq_t  sum = 0;
    seq_t  pw  = 1;          /* 3^k */

    while (t > 0) {
        int  d   = (int)(t % 9);   /* next digit in base 9 */
        sum += s_real[d] * pw;     /* add contribution */
        t   /= 9;                  /* move to next digit */
        pw  *= 3;                  /* next power of 3 */
    }
    return sum;
}
