/*
 * A259225  Smallest oblong number greater than or equal to n.
 * https://oeis.org/A259225
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest oblong number ≥ n
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Oblong numbers are k*(k+1).  For each n this returns the minimal
   k such that k*(k+1) ≥ n.  By definition a(0) = 0. */

seq_t a(idx_t n)
{
    /* Special case for n = 0. */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Find an upper bound for the desired k by exponential growth.
       The loop never exceeds 2^32 for 64‑bit idx_t, so it is safe. */
    idx_t high = 1;
    while ((seq_t)high * (high + 1) < n) {
        high <<= 1;   /* double the bound */
    }

    /* Binary search for the smallest k with k*(k+1) ≥ n. */
    idx_t low = 1;
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        seq_t prod = (seq_t)mid * (mid + 1);

        if (prod >= n) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }

    /* Compute and return the oblong number for the found k. */
    return (seq_t)low * (low + 1);
}
