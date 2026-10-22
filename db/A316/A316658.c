/*
 * A316658  For any n >= 0 with base-5 expansion Sum_{k=0..w} d_k * 5^k, let
 *          f(n) = Sum_{k=0..w} [d_k > 0] * (2 + i)^k * i^(d_k - 1) (where []
 *          is an Iverson bracket and i denotes the imaginary unit); a(n)
 *          equals the imaginary part of f(n).
 * https://oeis.org/A316658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look, base
 * Formula:   a(n) = Imaginary part of \sum_{k\ge 0} [d_k>0]\, (2+i)^k\,
 *            i^{\,d_k-1}, where d_k are the base‑5 digits of n (least
 *            significant first)
 * Range:     i8 n <= 9500, i16 n <= 9500, i32 n <= 9500, i64 n <= 9500,
 *            i128 n <= 9500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    seq_t res_real = 0, res_imag = 0;   /* accumulators for real and imaginary parts */
    seq_t A = 1, B = 0;                 /* (2+i)^k = A + i B for current k */

    while (n > 0) {
        idx_t d = n % 5;                /* current base‑5 digit */
        n /= 5;

        if (d > 0) {
            seq_t term_real, term_imag;
            /* i^(d-1) : 0->1, 1->i, 2->-1, 3->-i */
            switch (d - 1) {
                case 0: term_real = A;          term_imag = B;          break; /* 1       */
                case 1: term_real = -B;         term_imag = A;          break; /* i       */
                case 2: term_real = -A;         term_imag = -B;         break; /* -1      */
                case 3: term_real = B;          term_imag = -A;         break; /* -i      */
                default: term_real = 0;         term_imag = 0;          break; /* unreachable */
            }
            res_real += term_real;
            res_imag += term_imag;
        }

        /* multiply current (A,B) by (2+i) to get the next power */
        seq_t newA = A * (seq_t)2 - B;  /* 2A - B   */
        seq_t newB = B * (seq_t)2 + A;  /* 2B + A   */
        A = newA;
        B = newB;
    }

    return res_imag;
}
