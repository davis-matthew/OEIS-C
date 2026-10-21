/*
 * A283972  a(n) = n minus (product of lengths of 1-runs in binary
 *          representation of n) = n - A227349(n).
 * https://oeis.org/A283972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n - (product of lengths of 1‑runs in binary
 *            representation of n)
 * Range:     u8 n <= 256, u16 n <= 9279, u32 n <= 9279, u64 n <= 9279,
 *            u128 n <= 9279, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)      /* safety for n < 1, although the sequence starts at 1 */
        return 0;

    idx_t m       = n;         /* work on a copy to avoid modifying the caller's value */
    seq_t product = 1;         /* product of run lengths, start with multiplicative identity */
    unsigned run  = 0;         /* length of the current run of consecutive 1s */

    while (m) {
        if (m & 1) {           /* current bit is 1 – extend the run */
            run++;
        } else {               /* current bit is 0 – end of a run (if any) */
            if (run) {
                product *= (seq_t)run;
                run = 0;
            }
        }
        m >>= 1;               /* shift to process the next bit */
    }

    /* If the binary representation ends with a run of 1s, account for it */
    if (run)
        product *= (seq_t)run;

    return (seq_t)n - product;
}
