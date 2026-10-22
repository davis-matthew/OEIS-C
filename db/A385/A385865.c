/*
 * A385865  Triangle read by rows where T(n,k), for 1 <= k < n, is the column
 *          number where (n-k)^2 occurs in an n X n grid filled rowwise with
 *          the numbers 1 to n^2.
 * https://oeis.org/A385865
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = ((k^2-1) mod r) + 1 where r is the row index that
 *            contains the (n‑2)-th element of the flattened triangle and k (1
 *            ≤ k < r) is that element’s column number. The row r satisfies
 *            r(r‑1)/2 > n‑2, and k = n‑2 – (r‑2)(r‑1)/2 + 1
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External types `seq_t` and `idx_t` are provided by the build system. */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;          /* sequence is defined only for n ≥ 2 */

    /* zero‑based position within the concatenated rows */
    idx_t t = n - 2;

    /* Determine the row r containing this element.
       Linear search up to sqrt(2*t) is fast enough for the expected input range.  */
    idx_t r = 2;                          /* minimal possible r */
    while ( ((unsigned __int128)(r - 1) * r) / 2 <= t )
        ++r;

    /* Index within the row (0‑based) */
    idx_t p = t - ((r - 2) * (r - 1)) / 2;

    /* Column number k (1‑indexed) */
    idx_t k = p + 1;

    /* Compute T(r, k) = ((k^2 - 1) mod r) + 1 */
    seq_t kk = (seq_t)k;
    seq_t rr = (seq_t)r;
    return ( (kk * kk - (seq_t)1) % rr ) + (seq_t)1;
}
