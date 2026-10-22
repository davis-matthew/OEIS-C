/*
 * A335003  Triangle read by rows where the n-th row is the cycle trajectory
 *          of 2^n+1 in the divide-or-choose 2 rule.
 * https://oeis.org/A335003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n-th element of the triangle whose r-th row (r ≥ 1)
 *            consists of the cycle trajectory of 2^r + 1 under the
 *            divide‑or‑choose‑2 rule: - if x is even, f(x) = x / 2 - if x is
 *            odd, f(x) = binomial(x, 2) = x·(x–1)/2 The r-th row has r terms:
 *            S = 2^r + 1 S, S·2^(r–1), S·2^(r–2), … , S·2^1 The...
 * Range:     u8 n <= 11, u16 n <= 37, u32 n <= 137, u64 n <= 529,
 *            u128 n <= 2081, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the row r such that the cumulative number of terms
       up to that row is at least n.  (Rows are 1‑based.) */
    idx_t r = 1;
    idx_t sum = 1;   /* sum of the first r integers */
    while (sum < n) {
        ++r;
        sum += r;
    }

    /* Determine position within the row (0‑based). */
    idx_t prev = sum - r;          /* terms in all previous rows */
    idx_t pos  = n - prev - 1;     /* position inside current row */

    /* Compute S = 2^r + 1 */
    seq_t val = 1;                 /* start from 1 (2^0) */
    for (idx_t i = 0; i < r; ++i)
        val <<= 1;                 /* val = 2^r */
    val += 1;                      /* val = 2^r + 1 */

    /* If we are on the first position, return S directly. */
    if (pos == 0)
        return val;

    /* Otherwise multiply by the appropriate power of two.
       That power is 2^(r - pos). */
    idx_t exp = r - pos;
    for (idx_t i = 0; i < exp; ++i)
        val <<= 1;                 /* multiply by 2 each iteration */

    return val;
}
