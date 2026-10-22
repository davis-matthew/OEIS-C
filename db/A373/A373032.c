/*
 * A373032  Expansion of Sum_{k>=1} (-1)^(k+1) * k^2 * x^(k*(k+1)) / (1 -
 *          x^k).
 * https://oeis.org/A373032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n, d<sqrt(n)} (-1)^(d+1) * d^2 (d ranges over
 *            positive divisors of n not exceeding sqrt(n); the divisor equal
 *            to sqrt(n) is omitted when n is a perfect square)
 * Range:     i8 n <= 70, i16 n <= 70, i32 n <= 70, i64 n <= 70, i128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* offset starts at 1 */

    seq_t sum = 0;

    /* Compute floor(sqrt(n)) using an integer loop with 128‑bit intermediate
       to avoid overflow. */
    idx_t sqrt_n = 0;
    while ((__int128)(sqrt_n + 1) * (sqrt_n + 1) <= n)
        ++sqrt_n;

    for (idx_t d = 1; d <= sqrt_n; ++d)
    {
        if (n % d != 0) continue;                 /* not a divisor */

        /* Skip the divisor equal to sqrt(n) when n is a perfect square. */
        if ((__int128)d * d == n) continue;

        seq_t d2 = (seq_t)((__int128)d * d);      /* d^2 in seq_t */

        /* (-1)^(d+1) is +1 if d is odd, –1 if d is even. */
        if (d & 1)
            sum += d2;
        else
            sum -= d2;
    }

    return sum;
}
