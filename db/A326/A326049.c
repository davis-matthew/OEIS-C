/*
 * A326049  a(n) = n - A050449(n), where A050449 is the sum of divisors of the
 *          form 4k+1.
 * https://oeis.org/A326049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n - A050449(n) where A050449(n) = sum of all positive
 *            divisors d of n such that d ≡ 1 (mod 4)
 * Range:     i8 n <= 130, i16 n <= 8791, i32 n <= 8791, i64 n <= 8791,
 *            i128 n <= 8791, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A326049. */
seq_t a(idx_t n)
{
    if (n == 0) /* safety: offset starts at 1, but handle 0 gracefully */
        return 0;

    seq_t n_val = (seq_t)n;
    seq_t sum   = 0;

    /* Enumerate all divisors d of n. Since d <= n/d, the loop bound avoids overflow. */
    for (idx_t d = 1; d <= n / d; ++d)
    {
        if (n % d == 0)
        {
            if ((d & 3) == 1)          /* d ≡ 1 (mod 4) */
                sum += (seq_t)d;

            idx_t e = n / d;           /* complementary divisor */
            if (e != d && (e & 3) == 1)
                sum += (seq_t)e;
        }
    }

    return n_val - sum;   /* result may be negative */
}
