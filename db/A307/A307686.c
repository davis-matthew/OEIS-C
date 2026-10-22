/*
 * A307686  Sum of the smallest side lengths of all integer-sided triangles
 *          with perimeter n.
 * https://oeis.org/A307686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sum of the smallest side lengths of all integer‑sided triangles
   with perimeter n:
   a(n) = Σ_{a=1}^{⌊n/3⌋} a · max(0, ⌊(n−a)/2⌋ − max(a, ⌊(n−2a)/2⌋+1)+1)   */
seq_t a(idx_t n)
{
    if (n < 3)
        return 0;

    seq_t sum = 0;

    for (idx_t a_val = 1; a_val <= n / 3; ++a_val)
    {
        /* lower bound for b:  b > n/2 − a   →  b >= floor((n−2a)/2)+1   */
        idx_t low = (n - 2 * a_val) / 2 + 1;
        if (low < a_val)
            low = a_val;

        /* upper bound for b:  b ≤ (n−a)/2   */
        idx_t high = (n - a_val) / 2;

        if (high >= low)
        {
            idx_t cnt = high - low + 1;
            sum += (seq_t)a_val * (seq_t)cnt;
        }
    }

    return sum;
}
