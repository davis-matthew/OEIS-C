/*
 * A332412  a(n) is the real part of f(n) = Sum_{d_k > 0} 3^k * i^(d_k-1)
 *          where Sum_{k >= 0} 5^k * d_k is the base 5 representation of n and
 *          i denotes the imaginary unit. Sequence A332413 gives imaginary
 *          parts.
 * https://oeis.org/A332412
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   a(0) = 0; a(5*n) = 3*a(n); a(5*n+1) = 3*a(n)+1; a(5*n+2) =
 *            3*a(n); a(5*n+3) = 3*a(n)-1; a(5*n+4) = 3*a(n);
 * Range:     i8 n <= 3124, i16 n <= 9606, i32 n <= 9606, i64 n <= 9606,
 *            i128 n <= 9606, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t _f[5] = {0, 1, 0, (seq_t)-1, 0};

static seq_t a_rec(idx_t m) {
    if (m == 0) {
        return (seq_t)0;
    } else {
        seq_t val = a_rec(m / 5);
        val *= (seq_t)3;
        val += _f[m % 5];
        return val;
    }
}

seq_t a(idx_t n) {
    return a_rec(n);
}
