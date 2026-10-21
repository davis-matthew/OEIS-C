/*
 * A275539  a(n) = n! + n*(n-1)!!.
 * https://oeis.org/A275539
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! + n*(n-1)!!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i)
        fact *= (seq_t)i;          /* compute n! */

    seq_t dblfact = 1;
    if (n > 0) {
        idx_t i = n - 1;          /* start from n-1 */
        while (i >= 1) {
            dblfact *= (seq_t)i;  /* multiply by the next term of (n-1)!! */
            if (i <= 2)           /* avoid underflow on next i -= 2 */
                break;
            i -= 2;               /* step by two to keep parity */
        }
    }

    return fact + (seq_t)n * dblfact;  /* n! + n*(n-1)!! */
}
