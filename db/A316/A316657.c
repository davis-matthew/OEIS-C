/*
 * A316657  For any n >= 0 with base-5 expansion Sum_{k=0..w} d_k * 5^k, let
 *          f(n) = Sum_{k=0..w} [d_k > 0] * (2 + i)^k * i^(d_k - 1) (where []
 *          is an Iverson bracket and i denotes the imaginary unit); a(n)
 *          equals the real part of f(n).
 * https://oeis.org/A316657
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = Re( sum_{k>=0} [d_k>0] (2+i)^k * i^(d_k-1) ), where d_k
 *            are the base‑5 digits of n (least significant first)
 * Range:     i8 n <= 9758, i16 n <= 9758, i32 n <= 9758, i64 n <= 9758,
 *            i128 n <= 9758, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t re = 0;
    seq_t im = 0;
    seq_t pr = 1; /* (2+i)^0 real part */
    seq_t pi = 0; /* (2+i)^0 imaginary part */
    idx_t tmp = n;

    while (tmp)
    {
        idx_t d = tmp % 5;
        if (d)
        {
            seq_t term_re, term_im;
            idx_t off = d - 1;
            /* Multiply (pr + i*pi) by i^off */
            switch (off)
            {
                case 0: term_re = pr;            term_im = pi;          break;
                case 1: term_re = -pi;           term_im = pr;          break;
                case 2: term_re = -pr;           term_im = -pi;         break;
                case 3: term_re = pi;            term_im = -pr;         break;
            }
            re += term_re;
            im += term_im;
        }
        /* Update power: (pr + i*pi)*(2+i) = (2pr - pi) + i*(pr + 2pi) */
        seq_t new_pr = 2 * pr - pi;
        seq_t new_pi = pr + 2 * pi;
        pr = new_pr;
        pi = new_pi;

        tmp /= 5;
    }
    return re;
}
