/*
 * A385072  a(n) = number of tetrahedral numbers <= n-th triangular number.
 * https://oeis.org/A385072
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + max{k : k(k+1)(k+2)/6 <= n(n+1)/2}
 * Range:     u8 n <= 14, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 68 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t tetrahedron(idx_t k)
{
    seq_t a = k;
    seq_t b = k + 1;
    seq_t c = k + 2;

    /* divide by 2 */
    if ((a & 1) == 0)      a >>= 1;
    else if ((b & 1) == 0) b >>= 1;
    else                   c >>= 1;

    /* divide by 3 */
    if (a % 3 == 0)        a /= 3;
    else if (b % 3 == 0)   b /= 3;
    else                   c /= 3;

    return a * b * c;
}
/* Implements the sequence A385072 (number of tetrahedral numbers <= n-th triangular number) */

seq_t a(idx_t n)
{
    /* Compute the n-th triangular number */
    seq_t tri = (seq_t)n * ((seq_t)n + 1) / 2;

    /* Find an upper bound for the largest k with tetrahedron(k)<=tri */
    idx_t low  = 0;
    idx_t high = 1;
    while (tetrahedron(high) <= tri) {
        low = high;
        if (high == (idx_t)-1) break;   /* prevent wrap‑around */
        high <<= 1;
    }

    /* Binary search for the maximal k */
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        if (tetrahedron(mid) <= tri)
            low = mid;
        else
            high = mid - 1;
    }

    /* a(n) = number of tetrahedral numbers <= Tri(n) = low + 1 */
    return (seq_t)low + 1;
}
