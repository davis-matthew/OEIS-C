/*
 * A326781  No position of a 1 in the reversed binary expansion of n is a
 *          power of 2.
 * https://oeis.org/A326781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   No position of a 1 in the reversed binary expansion of n is a
 *            power of 2
 * Range:     u8 n <= 16, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n-th non‑negative integer having no bits set at positions
 * i = 0,1,3,7,15,31,...   (i+1 is a power of 2).       */
/* The value is obtained by mapping the binary digits of n‑1 into the
 * allowed positions.  Works for any integer types seq_t and idx_t. */

seq_t a(idx_t n)
{
    if (n == 0)               /* safety: offset is 1, but handle 0 */
        return 0;

    idx_t m = n - 1;          /* index in zero‑based order          */
    seq_t result = 0;

    /* number of binary digits of m */
    idx_t bits = 0, tmp = m;
    while (tmp) {
        tmp >>= 1;
        ++bits;
    }

    idx_t allowedPos = 0;     /* next allowed bit position in result */

    for (idx_t i = 0; i < bits; ++i) {
        /* skip forbidden positions (i+1 is a power of 2) */
        while (((allowedPos + 1) & allowedPos) == 0)
            ++allowedPos;

        if ((m >> i) & 1U)          /* set bit if corresponding bit of m is 1 */
            result |= ((seq_t)1 << allowedPos);

        ++allowedPos;                /* advance to next position for next bit   */
    }

    return result;
}
