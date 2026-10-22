/*
 * A397963  Value to the left of the rightmost longest series of 0-bits of n
 *          in binary.
 * https://oeis.org/A397963
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) returns the integer represented by the bits to the left *
 *            of the rightmost longest run of consecutive 0‑bits in the binary
 *            * representation of n. If n has no zero bits, the whole number
 *            is * returned; if n is 0 the result is 0
 * Range:     u8 n <= 510, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the trivial case n == 0 without looping. */
    if (n == 0) return (seq_t)0;

    idx_t temp = n;
    idx_t pos = 0;              /* current bit index (LSB = 0)      */
    idx_t longestLen = 0;       /* length of the longest zero run  */
    idx_t shift = 0;            /* (highest index of chosen run) + 1 */
    idx_t currentLen = 0;       /* length of the zero run currently seen */
    idx_t currentEnd = 0;       /* highest index of the current run   */

    /* Scan bits from LSB to MSB.  The first run of a given length
     * met while scanning from the LSB is the rightmost one, so we
     * only update when a strictly longer run is found. */
    while (temp != 0)
    {
        if ((temp & 1ULL) == 0)          /* zero bit */
        {
            currentLen++;
            currentEnd = pos;
        }
        else                              /* one bit ends the current run */
        {
            if (currentLen > 0)
            {
                if (currentLen > longestLen)
                {
                    longestLen = currentLen;
                    shift = currentEnd + 1;
                }
                currentLen = 0;
            }
        }
        temp >>= 1;
        pos++;
    }

    /* If the number ends with zeros, the last run is still pending. */
    if (currentLen > 0 && currentLen > longestLen)
    {
        shift = currentEnd + 1;
    }

    return (seq_t)(n >> shift);
}
