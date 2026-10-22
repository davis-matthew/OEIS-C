/*
 * A370700  Expansion of e.g.f. exp(-x^2)/(1-x).
 * https://oeis.org/A370700
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n!*Sum_{k=0..floor(n/2)} (-1)^k/k!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute n! */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i) {
        fact *= (seq_t)i;
    }

    seq_t current = fact;          /* current = n! / k!   (k starts at 0) */
    seq_t result  = 0;
    idx_t limit   = n / 2;         /* floor(n/2) */

    for (idx_t k = 0; k <= limit; ++k) {
        /* add or subtract the term */
        if ((k & 1ULL) == 0) {
            result += current;
        } else {
            result -= current;
        }

        /* prepare current for next k, except on the last loop */
        if (k < limit) {
            current /= (seq_t)(k + 1);   /* integer division is exact */
        }
    }

    return result;
}
