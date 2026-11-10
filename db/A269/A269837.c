/*
 * A269837  Irregular triangle read by rows: even terms of A094728(n+1)
 *          divided by 4.
 * https://oeis.org/A269837
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = (p-j)*(p+j+q) where g = largest integer with g*(g-1) <=
 *            n, r = n-g*(g-1), p = g, (q,j) = (0,r) if r < g else (1,r-g);
 *            i.e. even terms of A094728(n+1), divided by 4
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 64 terms through n = 63 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t lo = 1, hi = ((idx_t)1 << 32) + 1;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        unsigned __int128 t = (unsigned __int128)mid * (mid - 1);
        if (t <= (unsigned __int128)n)
            lo = mid;
        else
            hi = mid - 1;
    }
    idx_t g = lo;
    idx_t r = (idx_t)((unsigned __int128)n - (unsigned __int128)g * (g - 1));
    idx_t q, j;
    if (r < g) {
        q = 0;
        j = r;
    } else {
        q = 1;
        j = r - g;
    }
    seq_t p = (seq_t)g;
    seq_t jj = (seq_t)j;
    return (p - jj) * (p + jj + (seq_t)q);
}
