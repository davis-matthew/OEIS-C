/*
 * A318744  a(n) = Sum_{k=1..n} floor(n/k)^5.
 * https://oeis.org/A318744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} floor(n/k)^5
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 83, u64 n <= 7080,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t i = 1;
    while (i <= n) {
        idx_t v = n / i;                /* common floor value */
        idx_t j = n / v;                /* last index with this value */
        idx_t cnt = j - i + 1;          /* how many times it occurs */

        seq_t val = (seq_t)v;
        val *= (seq_t)v;      /* v^2 */
        val *= (seq_t)v;      /* v^3 */
        val *= (seq_t)v;      /* v^4 */
        val *= (seq_t)v;      /* v^5 */

        sum += val * (seq_t)cnt;          /* add contribution */

        i = j + 1;                     /* move to next block */
    }
    return sum;
}
