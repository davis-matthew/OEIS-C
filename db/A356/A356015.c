/*
 * A356015  a(n) = n! * Sum_{k=1..n} 1/(k * floor(n/k)!).
 * https://oeis.org/A356015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * sum_{k=1..n} 1/(k * floor(n/k)!)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation uses an incremental product to avoid storing large factorials. */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* not used, offset is 1 */
    seq_t sum = 0;
    idx_t curr_m = n;
    seq_t prod = 1;         /* prod = n! / curr_m!  (initially n! / n! = 1) */

    for (idx_t k = 1; k <= n; ++k) {
        idx_t new_m = n / k;
        /* Update prod for the current floor(n/k) value */
        while (curr_m > new_m) {
            curr_m--;                           /* move to a smaller m */
            prod = prod * (seq_t)(curr_m + 1);  /* multiply by the new factor */
        }
        /* prod now equals n! / curr_m! where curr_m == new_m */
        seq_t term = prod / (seq_t)k;           /* integer division is exact */
        sum += term;
    }
    return sum;
}
