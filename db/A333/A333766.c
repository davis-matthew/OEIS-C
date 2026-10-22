/*
 * A333766  Maximum part of the n-th composition in standard order. a(0) = 0.
 * https://oeis.org/A333766
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = longest run of 0's in binary expansion of n + 1 (for
 *            n>0); a(0)=0
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t maxrun = 0;   /* longest zero run found so far */
    seq_t cur    = 0;   /* current consecutive zero run length */

    idx_t temp = n;
    while (temp != 0) {
        if (temp & 1) {            /* bit is 1 */
            if (cur > maxrun)      /* finish current run */
                maxrun = cur;
            cur = 0;               /* reset current run */
        } else {                   /* bit is 0 */
            cur++;                 /* extend current zero run */
        }
        temp >>= 1;                /* next bit */
    }

    /* if number ends with zeros, update maxrun */
    if (cur > maxrun)
        maxrun = cur;

    return maxrun + 1;
}
