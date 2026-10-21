/*
 * A255308  Number of times log_2 can be applied to n until the result is not
 *          a power of 2. Here log_2 means the base-2 logarithm.
 * https://oeis.org/A255308
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = 1 + a(log_2(n)) if n is a power of two, 0 otherwise;
 *            a(0)=0 (a(0)=0, a(1)=1, a(2)=2, a(3)=0, a(4)=3, a(5)=0, a(6)=0,
 *            a(7)=0, a(8)=1, a(16)=4, ...)
 * Range:     u8 n <= 9340, u16 n <= 9340, u32 n <= 9340, u64 n <= 9340,
 *            u128 n <= 9340, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* base case: a(0) = 0 */
    if ((n & (n - 1)) != 0) return 0;      /* not a power of two → 0 */

    /* compute floor(log2(n)) using a small loop (n is a power of two) */
    idx_t k = 0;
    idx_t t = n;
    while ((t >>= 1) != 0) ++k;            /* k such that 2^k = n */

    /* recurrence: a(n) = 1 + a(k) */
    return 1 + a(k);
}
