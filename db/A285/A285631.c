/*
 * A285631  a(n) = negative of exponent of s(1) in factorization of s(n) where
 *          s(1)^2 * s(n+2) * s(n-2) = s(2)^2 * s(n+1) * s(n-1) - s(3) * s(1)
 *          * s(n)^2, s(n) = -s(-n) for all n in Z, with a(0) = -3, and s(1),
 *          s(2), s(3), s(4) are variables.
 * https://oeis.org/A285631
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(r) + 6*r*q + 57*q^2 where n = 19*q + r, 0 <= r < 19 *
 *            a(0) = -3, a(1) = -1, a(2) = 0, a(3) = 0, a(4) = 0, a(5) = 3, *
 *            a(6) = 5, a(7) = 6, a(8) = 8, a(9) = 12, a(10) = 15, * a(11) =
 *            17, a(12) = 21, a(13) = 26, a(14) = 30, * a(15) = 33, a(16) =
 *            39, a(17) = 45, a(18) = 50
 * Range:     i8 n <= 28, i16 n <= 455, i32 n <= 5000, i64 n <= 5000,
 *            i128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* base values: a(0) … a(18) */
    static const seq_t base[19] = {
        -3, /* a(0) */
        -1, /* a(1) */
        0,  /* a(2) */
        0,  /* a(3) */
        0,  /* a(4) */
        3,  /* a(5) */
        5,  /* a(6) */
        6,  /* a(7) */
        8,  /* a(8) */
        12, /* a(9) */
        15, /* a(10) */
        17, /* a(11) */
        21, /* a(12) */
        26, /* a(13) */
        30, /* a(14) */
        33, /* a(15) */
        39, /* a(16) */
        45, /* a(17) */
        50  /* a(18) */
    };
    idx_t q = n / 19;          /* quotient of n divided by 19   */
    idx_t r = n % 19;          /* remainder 0..18                */
    seq_t val = base[r];       /* constant part for the residue  */
    /* compute the linear and quadratic contributions in seq_t */
    seq_t term1 = (seq_t)6 * (seq_t)r * (seq_t)q;       /* 6*r*q   */
    seq_t term2 = (seq_t)57 * (seq_t)q * (seq_t)q;      /* 57*q^2  */
    return val + term1 + term2;
}
