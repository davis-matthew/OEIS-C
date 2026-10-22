/*
 * A348463  Size of largest bipartite biregular Moore graph of diameter 6 and
 *          degrees n and n.
 * https://oeis.org/A348463
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 2*(1 + (n-1) + (n-1)^2 + (n-1)^3 + (n-1)^4 + (n-1)^5) for
 *            n >= 2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)  /* behavior is undefined for n < 2, but return 0 for safety */
        return 0;

    seq_t base = (seq_t)(n - 1);   /* (n-1) */
    seq_t pow  = 1;                /* (n-1)^0 */
    seq_t sum  = 1;                /* start with 1 (k = 0) */

    /* accumulate powers up to (n-1)^5 */
    for (idx_t i = 1; i <= 5; ++i) {
        pow *= base;               /* pow = (n-1)^i */
        sum += pow;
    }
    return sum * 2;                /* final result */
}
