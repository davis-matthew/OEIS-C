/*
 * A290518  Minimum value of Product_{i in lambda} i!, where lambda ranges
 *          over all partitions of n into distinct parts.
 * https://oeis.org/A290518
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 23, u64 n <= 40, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A290518.  Minimal product of factorials of parts in
   a partition of n into distinct parts.
   Formula:  a(0) = 1,
   a(n) = a(n-1) * f(n)  for n>0,
   where f(n) = 1 if n is a triangular number,
   otherwise f(n) = (T_k - n + 1) with T_k the smallest
   triangular number greater than n (i.e. the next triangular number). */
seq_t a(idx_t n)
{
    seq_t res = (seq_t)1;        /* a(0) = 1 */
    if (n == 0) return res;

    idx_t cur_k = 0;             /* current k such that T_k = k*(k+1)/2 */
    idx_t cur_T = 0;             /* current triangular number T_k */

    for (idx_t m = 1; m <= n; ++m) {
        /* advance to the smallest triangle number >= m */
        while (m > cur_T) {
            ++cur_k;
            cur_T = cur_k * (cur_k + 1) / 2;
        }

        /* factor depends on whether m is triangular */
        seq_t factor = (m == cur_T) ? (seq_t)1 : (seq_t)(cur_T - m + 1);
        res *= factor;
    }
    return res;
}
