/*
 * A351381  Table read by downward antidiagonals: T(n,k) = n*(k+1)^2.
 * https://oeis.org/A351381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,c) with T(r,c) = r*(c+1)^2, where (r,c) are the
 *            coordinates read by downward antidiagonals of the table T(n,k)
 * Range:     u8 n <= 48, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) for n >= 1. */
seq_t a(idx_t n)
{
    /* Find diagonal s (sum of row+col) such that
       s*(s-1)/2 >= n  (minimal such s). */
    idx_t low  = 2;
    idx_t high = 2;
    while ((unsigned __int128)high * (high - 1) / 2 < n)
        high <<= 1;

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid - 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t s = low;          /* diagonal index (sum of coordinates) */

    /* Number of elements before diagonal s. */
    idx_t cumPrev =
        (idx_t)(((unsigned __int128)(s - 1) * (s - 2)) / 2);

    /* Offset within diagonal (0‑based). */
    idx_t d = n - cumPrev - 1;
    idx_t r = d + 1;       /* row index (1‑based) */
    idx_t c = s - r;       /* column index (1‑based) */

    /* Compute T(r,c) = r * (c+1)^2. */
    seq_t cp1 = (seq_t)(c + 1);
    seq_t res = (seq_t)r * (cp1 * cp1);
    return res;
}
