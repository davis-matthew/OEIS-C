/*
 * A368266  Expansion of e.g.f. exp(-2*x) / (1 - x*exp(x)).
 * https://oeis.org/A368266
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n! * Sum_{k=0..n} (n-k-2)^k / k!
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 10, i64 n <= 17, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A368266.  Works with signed seq_t of any width
 * (int8_t .. __int128 or an arbitrary‑precision class).  All indices
 * are 64‑bit unsigned `idx_t`.  The function is safe for any n≥0
 * and supports non‑increasing calls. */
seq_t a(idx_t n)
{
    /* n!  */
    seq_t fact_n = 1;
    for (idx_t i = 1; i <= n; ++i)
        fact_n *= (seq_t)i;

    seq_t sum   = 0;
    seq_t ratio = fact_n;            /* ratio = n! / k!  (0! = 1) */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* (n - k - 2) as a signed seq_t */
        seq_t base = (seq_t)((int64_t)n - (int64_t)k - 2);

        /* power = base^k  */
        seq_t power = 1;
        for (idx_t t = 0; t < k; ++t)
            power *= base;

        /* term = ratio * power = n! * base^k / k! */
        sum += ratio * power;

        /* prepare ratio for next k: divide by (k+1)  */
        if (k < n)
            ratio /= (seq_t)(k + 1);
    }
    return sum;
}
