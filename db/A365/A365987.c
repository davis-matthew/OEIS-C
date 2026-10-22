/*
 * A365987  Multiply each term by 3 and erase the rightmost digit of the
 *          result: this leaves the sequence unchanged.
 * https://oeis.org/A365987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = ceil(10*a(n-1)/3) with a(1) = 1
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 37, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;        /* undefined; harmless */

    seq_t cur = 1;             /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
    {
#if defined(__SIZEOF_INT128__) && !defined(__INTELLISENSE__)  /* compiler supports 128‑bit */
        /* Use 128‑bit intermediate when seq_t fits in 64 bits */
        if (sizeof(seq_t) <= 8)
        {
            __uint128_t tmp = (__uint128_t)cur * 10 + 2;
            cur = (seq_t)(tmp / 3);
        }
        else
#endif
        {
            /* seq_t is larger (128‑bit or arbitrary precision) – arithmetic
               won't overflow for those types, so use direct formula. */
            cur = (cur * (seq_t)10 + 2) / 3;
        }
    }
    return cur;
}
