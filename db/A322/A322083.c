/*
 * A322083  Square array A(n,k), n >= 1, k >= 0, read by antidiagonals: A(n,k)
 *          = Sum_{d|n} (-1)^(n/d+d)*d^k.
 * https://oeis.org/A322083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = Sum_{d|i} (-1)^(i/d + d) * d^k, where (i,k) are the
 *            antidiagonal coordinates of n (i>=1, k>=0)
 * Range:     i8 n <= 30, i16 n <= 69, i32 n <= 158, i64 n <= 414,
 *            i128 n <= 1138, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A322083. */
seq_t a(idx_t n)
{
    /* Find the diagonal index s such that n <= s(s+1)/2. */
    idx_t s = 1;
    while ((s * (s + 1)) / 2 < n) {
        ++s;
    }

    /* Elements before the current diagonal. */
    idx_t before = (s - 1) * s / 2;
    /* Position within the diagonal. */
    idx_t offset = n - before - 1;

    /* Anti‑diagonal coordinates: i = offset+1,  k = s-1-offset. */
    idx_t i = offset + 1;
    idx_t k = s - 1 - offset;

    seq_t sum = 0;

    /* Iterate over divisors of i. */
    for (idx_t d = 1; d * d <= i; ++d) {
        if (i % d == 0) {
            /* First divisor d */
            idx_t parity = ((i / d + d) & 1U);
            seq_t sign = parity ? (seq_t)(-1) : (seq_t)1;
            seq_t pow = 1;
            for (idx_t t = 0; t < k; ++t) {
                pow *= (seq_t)d;
            }
            sum += sign * pow;

            /* Second divisor e if distinct */
            idx_t e = i / d;
            if (e != d) {
                parity = ((i / e + e) & 1U);
                sign = parity ? (seq_t)(-1) : (seq_t)1;
                pow = 1;
                for (idx_t t = 0; t < k; ++t) {
                    pow *= (seq_t)e;
                }
                sum += sign * pow;
            }
        }
    }

    return sum;
}
