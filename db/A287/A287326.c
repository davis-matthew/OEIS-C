/*
 * A287326  Triangle read by rows: T(n, k) = 6*k*(n-k) + 1; n >= 0, 0 <= k <=
 *          n.
 * https://oeis.org/A287326
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 6*k*(r‑k)+1, where the flattened triangle index n
 *            satisfies n = r*(r+1)/2 + k with 0 ≤ k ≤ r. This implements OEIS
 *            sequence A287326
 * Range:     u8 n <= 109, u16 n <= 495, u32 n <= 495, u64 n <= 495,
 *            u128 n <= 495, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
    idx_t low  = 0;
    idx_t high = n;                /* upper bound is at most n */
    while (low < high) {
        idx_t mid = low + ((high - low + 1) >> 1);   /* upper middle */
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low;                                         /* row number */
    idx_t tri = (idx_t)((unsigned __int128)r * (r + 1) / 2);
    idx_t k   = n - tri;                                   /* offset in row */

    /* Compute the triangle element T(r, k) = 6*k*(r‑k)+1 */
    seq_t result = (seq_t)6 * (seq_t)k * ((seq_t)r - (seq_t)k) + (seq_t)1;
    return result;
}
