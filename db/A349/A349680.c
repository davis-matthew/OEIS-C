/*
 * A349680  a(n) = Sum_{k=1..n} (n-k)^c(n/k), where c(n) = 1 - ceiling(n) +
 *          floor(n).
 * https://oeis.org/A349680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} { (n-k) if k|n else 1 }
 * Range:     u8 n <= 35, u16 n <= 1979, u32 n <= 9762, u64 n <= 9762,
 *            u128 n <= 9762, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset 1, but guard for safety */
    seq_t sum = 0;
    seq_t seq_n = (seq_t)n;
    for (idx_t k = 1; k <= n; ++k)
    {
        if (n % k == 0)
            sum += seq_n - (seq_t)k;   /* (n-k) when k divides n */
        else
            sum += 1;                 /* otherwise add 1 */
    }
    return sum;
}
