/*
 * A262523  a(n+3) = a(n) + 6*n + 13, a(0)=0, a(1)=2, a(2)=7.
 * https://oeis.org/A262523
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A262523: a(n) = a(n-3) + 6*n - 5  (n ≥ 3),
   a(0)=0, a(1)=2, a(2)=7.
   Closed form:
       let n = 3*m + r   (0 ≤ r < 3)
       a(n) = 9*m^2 + (4 + 6*r)*m + a(r)   with a(0)=0, a(1)=2, a(2)=7 */

seq_t a(idx_t n)
{
    idx_t m = n / 3;          /* quotient */
    idx_t r = n % 3;          /* remainder */

    seq_t mseq = (seq_t)m;    /* convert to seq_t for arithmetic */
    seq_t result = 9 * mseq * mseq;  /* 9*m^2 term */

    /* add linear part and base value */
    if (r == 0) {
        result += 4 * mseq;               /* a(0) = 0 */
    } else if (r == 1) {
        result += 10 * mseq + 2;          /* a(1) = 2 */
    } else { /* r == 2 */
        result += 16 * mseq + 7;          /* a(2) = 7 */
    }

    return result;
}
