/*
 * A351824  Irregular triangle read by rows: T(n,k) is the number of
 *          partitions of n into 2*k-1 consecutive parts, n >= 1, k >= 1.
 *          Column k lists 1's interleaved with 2*k-2 zeros, and the first
 *          element of column k is in row A000384(k).
 * https://oeis.org/A351824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = 1 if the element at position n in the irregular triangle
 *            read by rows satisfies that the row number is divisible by the
 *            odd divisor (2·col−1), otherwise 0
 * Range:     u8 n <= 8168, u16 n <= 8168, u32 n <= 8168, u64 n <= 8168,
 *            u128 n <= 8168, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t isqrt_u64(idx_t x)
{
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= x) ++r;   /* integer square root */
    return r;
}

static inline idx_t triangle_row_len(idx_t r)
{
    /* len(r) = floor((sqrt(8*r+1)+1)/4) */
    idx_t s = isqrt_u64(8 * r + 1);
    return (s + 1) / 4;
}

extern seq_t a(idx_t n)
{
    idx_t cum   = 0;          /* cumulative number of entries up to previous row */
    idx_t row   = 1;          /* current row number being examined */

    /* Find the row that contains the nth element */
    for (;; ++row)
    {
        idx_t len = triangle_row_len(row);
        if (cum + len >= n) break;
        cum += len;
    }

    /* Column within the found row (1‑based) */
    idx_t col = n - cum;          /* 1‑based column index */

    /* Test if row is divisible by the odd divisor (2*col-1) */
    idx_t odd_divisor = (col << 1) - 1;   /* 2*col - 1 */
    seq_t result = (row % odd_divisor == 0) ? (seq_t)1 : (seq_t)0;

    return result;
}
