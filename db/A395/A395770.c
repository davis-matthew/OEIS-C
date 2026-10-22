/*
 * A395770  Minimal value of Sum_{i=1..n-1} p(i)*p(i+1), as p ranges over all
 *          permutations of powers of 2 {1,2,4,...,2^(n-1)}.
 * https://oeis.org/A395770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 2^(n-5) * (10*n - 5 + (-1)^n) for n >= 2, a(1) = 0
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 59, u128 n <= 122,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety for out‑of‑range call */
    if (n == 1) return (seq_t)0;   /* a(1) = 0 */

    /* numerator = 10*n - 5 + (-1)^n */
    seq_t num = (seq_t)(10 * n - 5);
    if ((n & 1) == 0)   /* n even -> (-1)^n = +1 */
        num += (seq_t)1;
    else
        num += (seq_t)(-1);  /* n odd  -> (-1)^n = -1 */

    if (n >= 5) {
        /* multiply by 2^(n-5) */
        num <<= (n - 5);
    } else {
        /* divide by 2^(5-n)  (right shift) */
        num >>= (5 - n);
    }

    return num;
}
