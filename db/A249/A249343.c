/*
 * A249343  The exponent of the highest power of 3 dividing the product of the
 *          elements on the n-th row of Pascal's triangle (A001142(n)).
 * https://oeis.org/A249343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..floor(log_3(n))} t * ((t+1)*3^k - n - 1), where
 *            t = floor(n / 3^k)
 * Range:     u8 n <= 80, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t power3k = 3;                     /* 3^1 */

    /* Iterate over powers of 3 while they do not exceed n. */
    while (power3k <= n) {
        idx_t t = n / power3k;             /* floor(n / 3^k) */

        /* Compute (t+1)*3^k - n - 1 safely as seq_t. */
        seq_t term = (seq_t)(t + 1) * (seq_t)power3k;
        term = term - (seq_t)n - 1;

        result += (seq_t)t * term;

        /* Prepare next power of 3; avoid overflow of power3k. */
        if (power3k > n / 3) break;
        power3k *= 3;
    }

    return result;
}
