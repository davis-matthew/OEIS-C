/*
 * A364761  G.f. satisfies A(x) = 1 / (1 + x*(1 + x*A(x))^3).
 * https://oeis.org/A364761
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 1/(n+1) * Sum_{k=0..n} (-1)^k * binomial(n+1,k) *
 *            binomial(3*k, n-k)
 * Range:     i8 n <= 1, i16 n <= 5, i32 n <= 12, i64 n <= 27, i128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    seq_t sum = 0;
    idx_t k_min = (n + 3) / 4;          /* ceil(n/4) */
    seq_t B = 0;                        /* binomial(3*k, n-k) */
    seq_t c1 = 1;                       /* binomial(n+1, 0) */

    for (idx_t k = 0; k <= n; ++k) {
        if (k == k_min) {
            /* compute binomial(3k, n-k) directly */
            idx_t t = n - k;
            B = 1;
            for (idx_t i = 1; i <= t; ++i) {
                B *= (seq_t)(3 * k - i + 1);
                B /= (seq_t)i;
            }
        } else if (k > k_min) {
            /* update B using recurrence:
               B(k+1) = B(k) * (3k+3)(3k+2)(3k+1)(n-k) /
                            (4k-n+1)(4k-n+2)(4k-n+3)(4k-n+4) */
            idx_t kp = k - 1;
            seq_t numer = (seq_t)(3 * kp + 3);
            numer *= (seq_t)(3 * kp + 2);
            numer *= (seq_t)(3 * kp + 1);
            numer *= (seq_t)(n - kp);

            seq_t denom = 1;
            denom *= (seq_t)(4 * kp - n + 1);
            denom *= (seq_t)(4 * kp - n + 2);
            denom *= (seq_t)(4 * kp - n + 3);
            denom *= (seq_t)(4 * kp - n + 4);

            B = B * numer / denom;
        }
        /* accumulate the signed term */
        if ((k & 1) == 0)
            sum += c1 * B;
        else
            sum -= c1 * B;

        /* update binomial coefficient for next k */
        if (k < n) {
            c1 = c1 * (seq_t)(n + 1 - k) / (seq_t)(k + 1);
        }
    }
    return sum / (seq_t)(n + 1);
}
