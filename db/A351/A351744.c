/*
 * A351744  Increment all even digits of n.
 * https://oeis.org/A351744
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number obtained from the decimal representation of n by
 *            incrementing each even digit by 1 (1,3,5,7,9 unchanged)
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)                /* a(0) = 1 */
        return (seq_t)1;

    idx_t  mult = 1;           /* 10^position */
    seq_t  res  = 0;

    while (n > 0) {
        idx_t d = n % 10;      /* current digit */
        n    /= 10;            /* drop processed digit */

        if ((d & 1) == 0)      /* even digit? */
            d++;               /* increment by 1 */

        res += (seq_t)d * (seq_t)mult;   /* add processed digit */
        mult *= 10;                   /* next positional weight */
    }

    return res;
}
