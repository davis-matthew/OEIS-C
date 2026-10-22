/*
 * A345026  Total area of all i X j rectangles where i and j are the final
 *          digits of positive integers r and s such that r + s = n.
 * https://oeis.org/A345026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..floor(n/2)} (k mod 10) * ((n-k) mod 10)
 * Range:     u8 n <= 29, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t sum = 0;
    idx_t m = n >> 1;          /* floor(n/2) */
    idx_t d = n % 10;          /* n mod 10 */

    for (int r = 0; r <= 9; ++r) {
        idx_t count;
        if (r == 0) {
            count = m / 10;                  /* k = 10,20,... */
        } else {
            if (m < (idx_t)r) continue;      /* no such k */
            count = (m - (idx_t)r) / 10 + 1; /* count of k ≡ r (mod 10) */
        }

        int val = (int)((d + 10 - r) % 10);   /* (n - r) mod 10 */
        seq_t term = (seq_t)r * (seq_t)val;
        sum += (seq_t)count * term;
    }

    return sum;
}
