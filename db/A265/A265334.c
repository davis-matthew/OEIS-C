/*
 * A265334  Numbers that are >= k! but < 2*k! for some k; numbers whose
 *          factorial base representation (A007623) begins with digit "1".
 * https://oeis.org/A265334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth integer whose factorial base representation *
 *            begins with digit 1, i.e., the nth integer in the union of
 *            ranges * [k!, 2*k!‑1] for k ≥ 1
 * Range:     u8 n <= 153, u16 n <= 5915, u32 n <= 5915, u64 n <= 5915,
 *            u128 n <= 5915, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset is 1, but guard anyway */
    seq_t fact = 1;                 /* k! for current k, starts at 1! */
    seq_t sum  = 0;                 /* cumulative count Σ_{i<k} i! */
    idx_t  k   = 1;
    while (sum + fact < (seq_t)n)
    {
        sum += fact;                /* add previous block sizes */
        ++k;                         /* move to next k */
        fact *= (seq_t)k;            /* compute k! */
    }
    seq_t pos = (seq_t)n - sum - 1; /* zero‑based offset inside this block */
    return fact + pos;              /* value in [k!, 2*k!‑1] */
}
