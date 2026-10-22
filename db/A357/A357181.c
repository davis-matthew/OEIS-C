/*
 * A357181  Last run-length of the n-th composition in standard order.
 * https://oeis.org/A357181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A357181 – Last run‑length of the n‑th composition in standard order.
 *
 *  The n‑th composition is obtained by the bijection of A066099:
 *  take the binary representation of n, read it least‑significant‑bit first
 *  (i.e. reverse it), prepend a 0, take first differences and reverse again.
 *  The last run‑length of the composition equals the length of the
 *  initial run of equal differences produced this way.
 *
 *  This implementation follows the definition directly.
 */

seq_t a(idx_t n)
{
    if (n == 0)                 /* a(0) = 0 by convention */
        return 0;

    /* Keep a copy of the original index for bit tests later. */
    const idx_t orig = n;

    /* Find the position of the first set bit (count trailing zeros). */
    idx_t tmp = n;
    idx_t tz  = 0;
    while ((tmp & 1ULL) == 0) {
        ++tz;
        tmp >>= 1;
    }

    /* First difference: number of zeros before the first 1 plus one. */
    seq_t d0   = (seq_t)(tz + 1);
    seq_t run  = 1;               /* at least one part in the composition */

    idx_t prev = tz + 1;          /* position of the previous 1 (add 1). */

    /* Scan the remaining set bits of the original number. */
    for (idx_t i = tz + 1; (orig >> i) != 0; ++i) {
        if (((orig >> i) & 1ULL) != 0) {
            idx_t cur   = i + 1;                 /* position of this 1 after shift */
            seq_t diff  = (seq_t)(cur - prev);   /* current difference */
            if (diff == d0) {                    /* still part of the first run? */
                ++run;
                prev = cur;
            } else {
                break;                           /* run finished */
            }
        }
    }

    return run;
}
