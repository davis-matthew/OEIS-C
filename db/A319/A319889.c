/*
 * A319889  a(n) = 6*5*4*3*2*1 - 12*11*10*9*8*7 + 18*17*16*15*14*13 -
 *          24*23*22*21*20*19 + ... - (up to the n-th term).
 * https://oeis.org/A319889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^floor(n/6) * Σ_{j=1..5} [I((n-j) mod 6 == 0) * *
 *            Π_{i=1..j} (n-i-j+7)] + Σ_{m=1..floor(n/6)} [(-1)^(m+1) * *
 *            Π_{t=0..5} (6m - t)]
 * Range:     i8 n <= 3, i16 n <= 10, i32 n <= 31, i64 n <= 31, i128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const idx_t K = 6;
    idx_t q = n / K;          /* floor(n/k) */
    idx_t r = n % K;          /* n mod k */

    seq_t S1 = 0;
    if (r != 0) {
        seq_t sign1 = (q % 2 == 0) ? (seq_t)1 : -(seq_t)1;
        seq_t prod = 1;
        for (idx_t i = 1; i <= r; ++i) {
            seq_t term = (seq_t)n - (seq_t)i - (seq_t)r + (seq_t)K + (seq_t)1;
            prod *= term;
        }
        S1 = prod * sign1;
    }

    seq_t S2 = 0;
    for (idx_t m = 1; m <= q; ++m) {
        idx_t j = m * K;        /* j = m*k */
        seq_t prod = 1;
        for (idx_t t = 0; t < K; ++t) {
            seq_t term = (seq_t)j - (seq_t)t;
            prod *= term;
        }
        seq_t sign = (m % 2 == 1) ? (seq_t)1 : -(seq_t)1;
        S2 += prod * sign;
    }

    return S1 + S2;
}
