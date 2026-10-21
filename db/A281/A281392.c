/*
 * A281392  Number of occurrences of "01" as a subsequence in the binary
 *          expansion of n.
 * https://oeis.org/A281392
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of occurrences of the subsequence "01" in the
 *            binary expansion of n with a leading zero. The implementation
 *            scans the bits of n from least‑significant to most‑significant,
 *            maintaining a count of ones seen to the right. For each zero
 *            encountered it adds that count to the total. Final...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) is 0, because the expansion "00" contains no "01" subsequence. */
    if (n == 0)
        return 0;

    seq_t ones_seen = 0;   /* number of 1s to the right of the current bit */
    seq_t result    = 0;   /* accumulated count of "01" pairs */

    idx_t x = n;
    while (x) {
        if (x & 1ULL) {
            ++ones_seen;          /* a 1 is added to the suffix count */
        } else {
            result += ones_seen;  /* a 0 adds all suffix ones to result */
        }
        x >>= 1;
    }

    /* Implicit leading zero contributes all ones found in n. */
    result += ones_seen;

    return result;
}
