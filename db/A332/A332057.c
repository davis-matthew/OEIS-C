/*
 * A332057  Partial sums (and absolute value of first differences) of A332056:
 *          if odd (resp. even) add (resp. subtract) the partial sum to get
 *          the next term.
 * https://oeis.org/A332057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(3k-2) = 2k-1, a(3k-1) = 4k-1, a(3k) = 2k (k >= 1)
 * Range:     u8 n <= 193, u16 n <= 9113, u32 n <= 9113, u64 n <= 9113,
 *            u128 n <= 9113, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the term for any n >= 1, using the 3‑quasi‑periodic formula. */
    switch (n % 3) {
        case 1: { /* n = 3k-2  ->  a = 2k-1 */
            idx_t k = (n + 2) / 3;
            return (seq_t)2 * (seq_t)k - 1;
        }
        case 2: { /* n = 3k-1  ->  a = 4k-1 */
            idx_t k = (n + 1) / 3;
            return (seq_t)4 * (seq_t)k - 1;
        }
        case 0: { /* n = 3k    ->  a = 2k   */
            idx_t k = n / 3;
            return (seq_t)2 * (seq_t)k;
        }
    }
    /* unreachable, but return 0 to satisfy the compiler */
    return 0;
}
