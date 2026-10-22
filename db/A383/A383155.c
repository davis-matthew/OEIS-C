/*
 * A383155  a(n) = Sum_{k=0..n} k^6 * (-1)^k * 3^(n-k) * binomial(n,k).
 * https://oeis.org/A383155
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2^(n-6) * (-4368*n + 7290*n^2 - 3555*n^3 + 645*n^4 -
 *            45*n^5 + n^6)
 * Range:     i8 n <= 1, i16 n <= 6, i32 n <= 16, i64 n <= 39, i128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the element type first to avoid narrowing. */
    seq_t n_seq = (seq_t)n;
    seq_t n2 = n_seq * n_seq;
    seq_t n3 = n2 * n_seq;
    seq_t n4 = n3 * n_seq;
    seq_t n5 = n4 * n_seq;
    seq_t n6 = n5 * n_seq;

    /* Compute the polynomial p(n). */
    seq_t p = (seq_t)-4368 * n_seq;
    p += (seq_t)7290 * n2;
    p -= (seq_t)3555 * n3;
    p += (seq_t)645 * n4;
    p -= (seq_t)45 * n5;
    p += n6;

    /* Multiply by 2^(n-6) (or divide by 2^(6-n)).  The loop avoids
       shifting by a too‑large amount, which would be undefined for small
       fixed‑width types. */
    if (n >= 6) {
        idx_t k = n - 6;
        seq_t pow2 = (seq_t)1;
        while (k--) {
            pow2 *= (seq_t)2;
        }
        return p * pow2;
    } else {
        idx_t k = 6 - n;
        seq_t pow2 = (seq_t)1;
        while (k--) {
            pow2 *= (seq_t)2;
        }
        return p / pow2;
    }
}
