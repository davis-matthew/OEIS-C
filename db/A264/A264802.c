/*
 * A264802  Position of the n largest occurrences of a shortest addition chain
 *          of length n in A003313, written as a triangle.
 * https://oeis.org/A264802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the position of the n-th largest occurrence of * a
 *            shortest addition chain of length r, where r is the unique
 *            integer * satisfying r × (r + 1) / 2 ≥ n. * If p = n – r × (r –
 *            1) / 2 (1 ≤ p ≤ r) then * a(n) = 2^r for p = 1, * a(n) = 2^(r‑1)
 *            + 2^(r‑p) for p > 1
 * Range:     u8 n <= 28, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t pow2(idx_t exp)
{
    /* Compute 2^exp; works for any seq_t that can be constructed from
     * an unsigned long long.  For exp large enough to overflow the
     * underlying type the result is not needed because in those cases
     * the value of a(n) exceeds the available size of seq_t. */
    return (seq_t)((unsigned long long)1 << exp);
}

seq_t a(idx_t n)
{
    /* Find the unique r with r*(r+1)/2 ≥ n. */
    idx_t r = 0;
    idx_t sum = 0;              /* sum = r*(r+1)/2 for current r */
    while (sum < n) {
        ++r;
        sum += r;
    }

    idx_t prev_sum = sum - r;   /* sum for r-1 */
    idx_t p = n - prev_sum;     /* 1 ≤ p ≤ r */

    if (p == 1) {
        return pow2(r);
    } else {
        seq_t high = pow2(r - 1);
        seq_t low  = pow2(r - p);
        return high + low;
    }
}
