/*
 * A340261  T(n, k) is the number of integers that are less than or equal to k
 *          that do not divide n. Triangle read by rows, for 0 <= k <= n.
 * https://oeis.org/A340261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   for the 1‑indexed index n let r be the smallest integer with
 *            r(r+1)/2 ≥ n and k = n – r(r‑1)/2. Then a(n) = |{ j : 1 ≤ j ≤ k
 *            , r mod j ≠ 0 }|, i.e. T(r,k)
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;      /* below offset; defined as 0 for safety */

    idx_t r = 0;          /* row number (1‑based) */
    idx_t sum = 0;        /* cumulative count of terms up to row r */

    /* Find smallest r with r(r+1)/2 ≥ n */
    while (sum < n) {
        ++r;
        sum += r;
    }

    /* Column index within this row (1‑based) */
    idx_t k = n - (r - 1) * r / 2;

    /* Count numbers j ≤ k that do not divide r */
    seq_t count = 0;
    for (idx_t j = 1; j <= k; ++j) {
        if (r % j != 0)
            ++count;
    }

    return count;
}
