/*
 * A325937  Expansion of Sum_{k>=1} (-1)^(k + 1) * x^(2*k) / (1 - x^k).
 * https://oeis.org/A325937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = (# odd proper divisors of n) – (# even proper divisors of
 *            n)
 * Range:     i8 n <= 9128, i16 n <= 9128, i32 n <= 9128, i64 n <= 9128,
 *            i128 n <= 9128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent: a(n) = -sum_{d|n, d<n} (-1)^d */
seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  For n = 0 return 0 safely. */
    if (n <= 1)
        return 0;

    seq_t cnt = 0;
    idx_t i;
    /* Iterate over divisor pairs (i, n/i) without overflow. */
    for (i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;
            /* d1 is always < n, because i starts at 1 and n > 1. */
            cnt += (d1 & 1) ? 1 : -1;
            /* d2 may equal d1; skip duplicate.  Skip if d2 == n. */
            if (d2 != d1 && d2 < n)
                cnt += (d2 & 1) ? 1 : -1;
        }
    }
    return cnt;
}
