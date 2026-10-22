/*
 * A318706  For any n >= 0 with base-9 representation Sum_{k=0..w} d_k * 9^k,
 *          let g(n) = Sum_{k=0..w} s(d_k) * 3^k (where s(0) = 0, s(1+2*j) =
 *          i^j and s(2+2*j) = i^j * (1+i) for any j > 0, and i denotes the
 *          imaginary unit); a(n) is the imaginary part of g(n).
 * https://oeis.org/A318706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   not stated
 * Range:     i8 n <= 6560, i16 n <= 6560, i32 n <= 6560, i64 n <= 6560,
 *            i128 n <= 6560, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = Σ_{k≥0} Im(s(d_k)) * 3^k,
 * where d_k are the base‑9 digits of n and
 * s(0)=0,
 * s(1+2j)=i^j,
 * s(2+2j)=i^j*(1+i).
 * The imaginary part Im(s(d)) is 0, 1, or –1 as follows:
 *   digit: 0 1 2 3 4 5 6 7 8
 *   Im(s): 0 0 1 1 1 0 –1 –1 –1
 */
seq_t a(idx_t n)
{
    static const int sign[9] = {0, 0, 1, 1, 1, 0, -1, -1, -1};

    seq_t sum   = 0;
    seq_t pow3  = 1;   /* 3^k for current digit position k */

    idx_t t = n;
    while (t > 0)
    {
        unsigned digit = (unsigned)(t % 9); /* base‑9 digit */
        int s = sign[digit];
        if (s != 0)
            sum += (seq_t)s * pow3;         /* accumulate contribution */

        pow3 *= 3;                         /* next power of 3 */
        t /= 9;                            /* move to next digit */
    }

    return sum;
}
