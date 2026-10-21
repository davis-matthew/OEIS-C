/*
 * A264756  An eventually quasilinear solution to Hofstadter's Q recurrence.
 * https://oeis.org/A264756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1, a(2) = 0; for n ≥ 3 a(3k) = 3k (i.e., a(n) = n when n
 *            mod 3 = 0) a(3k+1) = 3 (i.e., a(n) = 3 when n mod 3 = 1) a(3k+2)
 *            = 2 (i.e., a(n) = 2 when n mod 3 = 2)
 * Range:     u8 n <= 257, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)1;
    if (n == (idx_t)2) return (seq_t)0;
    /* n >= 3 */
    switch (n % (idx_t)3) {
        case 0: return (seq_t)n;   /* a(n) = n */
        case 1: return (seq_t)3;   /* a(n) = 3 */
        default: /* case 2 */ return (seq_t)2;   /* a(n) = 2 */
    }
}
