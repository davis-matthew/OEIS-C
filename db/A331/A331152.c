/*
 * A331152  Triangle read by rows: T(n,k) (n>=k>=1) = f(n,n-k+1) where f(n,k)
 *          = ceiling((n/k)*floor(n/k)).
 * https://oeis.org/A331152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   For n≥1, let r be the least integer with r*(r+1)/2≥n. Set k = n
 *            - (r-1)*r/2 (0‑based). Then a(n) = ceil((r/(r-k)) *
 *            floor(r/(r-k))), where r-k = r - (k+1) + 1 = r - k
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 ≥ n and (r‑1)*r/2 < n. */
    idx_t lo = 1, hi = 1;
    while ((unsigned __int128)hi * (hi + 1) / 2 < n)
        hi <<= 1;                       /* expand hi until bound satisfied */

    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if ((unsigned __int128)mid * (mid + 1) / 2 >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t r = lo;                       /* row number */

    /* Previous rows count */
    idx_t prev = (unsigned __int128)(r - 1) * r / 2;
    /* Offset within row (0‑based) */
    idx_t offset = n - prev - 1;
    /* Column index k in the triangle (1‑based) */
    idx_t k = offset + 1;

    /* Argument to f: k2 = r - k + 1 = n‑k+1 */
    idx_t k2 = r - k + 1;

    /* Compute f(r, k2) = ceil( (r/k2) * floor(r/k2) ) */
    seq_t m   = (seq_t)(r / k2);        /* floor(r/k2) */
    seq_t sr  = (seq_t)r;
    seq_t numer = sr * m + (seq_t)(k2 - 1); /* add k2-1 for ceiling */
    seq_t result = numer / (seq_t)k2;     /* integer division gives ceil */
    return result;
}
