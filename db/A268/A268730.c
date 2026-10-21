/*
 * A268730  a(n) = Product_{k = 0..n} 2*(8*k + 5).
 * https://oeis.org/A268730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{k = 0..n} 2*(8*k + 5)
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 5, u64 n <= 9, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 10, a(n) = 2*(8*n + 5)*a(n-1) for n >= 1 */

seq_t a(idx_t n)
{
    seq_t result = 10;                  /* a(0) */

    if (n == 0)
        return result;

    for (idx_t k = 1; k <= n; ++k) {
        /* compute 2 * (8*k + 5) as an idx_t first, then cast to seq_t */
        idx_t factor = 2 * (8 * k + 5);
        result *= (seq_t)factor;
    }

    return result;
}
