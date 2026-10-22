/*
 * A386407  a(n) = -floor(log(Integral_{x=2n^3+2n+2...oo} n^(-x^3)
 *          dx)/log(n)).
 * https://oeis.org/A386407
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*(n^3+n+1)^3 + 8 (for n ≥ 5); special cases: a(2)=10658,
 *            a(3)=238337, a(4)=2628081
 * Range:     u8 none, u16 n <= 2, u32 n <= 9, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* As n is unsigned, no negative indexing needed. */
    seq_t n_t = (seq_t)n;
    /* Compute t = n^3 + n + 1 */
    seq_t t = n_t * n_t * n_t + n_t + 1;

    /* Compute cube of t */
    seq_t t2 = t * t;
    seq_t t3 = t2 * t;

    /* Base result 8 * t^3 */
    seq_t res = (seq_t)8 * t3;

    /* Add the small constant correction for small n */
    if (n == 2)
        res += (seq_t)10;
    else if (n == 3 || n == 4)
        res += (seq_t)9;
    else
        res += (seq_t)8;   /* For n ≥ 5 */

    return res;
}
