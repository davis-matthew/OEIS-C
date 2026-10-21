/*
 * A284359  Double triangle (2*n+2 terms by row). Every row is 2*n + 1
 *          followed by 2*n + 1 times 2*n + 2.
 * https://oeis.org/A284359
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = 2*k+1 for the first entry of row k, 2*k+2 otherwise,
 *            where k = ⌊(√(4n+1)-1)/2⌋ (the row index that contains n)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the smallest k with (k+1)(k+2) > n (row where n lies). */
    idx_t low  = 0;
    idx_t high = n;                 /* high may be 0 when n==0 */

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 prod = (unsigned __int128)(mid + 1u) * (mid + 2u);
        if (prod > n) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }

    idx_t k      = low;               /* row index */
    idx_t prev   = k * (k + 1);       /* number of terms before this row */
    idx_t offset = n - prev;          /* position within the row */

    if (offset == 0) {
        return (seq_t)(2 * k + 1);
    } else {
        return (seq_t)(2 * k + 2);
    }
}
