/*
 * A277937  Number of runs of 1's of length 1 in the binary expansion of n.
 * https://oeis.org/A277937
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of isolated 1's in the binary representation of n
 *            * (i.e., runs of 1's of length exactly one)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Work with 64‑bit unsigned arithmetic. */
    uint64_t bits   = (uint64_t)n;
    uint64_t left   = bits << 1;
    uint64_t right  = bits >> 1;

    /* Bits that are 1 and have 0 on both sides. */
    uint64_t isolated = bits & ~(left | right);

    /* Count set bits in isolated. */
    unsigned long long cnt = 0;
    while (isolated) {
        isolated &= isolated - 1; /* clear lowest set bit */
        ++cnt;
    }
    return (seq_t)cnt;
}
