/*
 * A362072  Antidiagonal sums of A336225.
 * https://oeis.org/A362072
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 2433, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formulas:
   a(n) = Sum_{k=1..n} A007953(k*(n-k))
   A007953(x) = sum of decimal digits of x.
   For n = 0, a(0) = 0. */

seq_t a(idx_t n)
{
    /* Accumulate the sum using an unsigned 64‑bit integer to avoid overflow
       in practical ranges; cast to seq_t for the final return. */
    idx_t total = 0;

    for (idx_t k = 1; k <= n; ++k)
    {
        /* Compute k*(n-k) using idx_t (uint64_t) to guarantee exactness. */
        idx_t prod = k * (n - k);

        /* Sum the decimal digits of prod. */
        idx_t digit_sum = 0;
        idx_t t = prod;
        while (t > 0)
        {
            digit_sum += t % 10;
            t /= 10;
        }

        total += digit_sum;
    }

    return (seq_t)total;
}
