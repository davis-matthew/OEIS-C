/*
 * A342410  The binary expansion of a(n) corresponds to that of n where all
 *          the 1's have been replaced by 0's except in the last run of 1's.
 * https://oeis.org/A342410
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the value represented by the right‑most contiguous run of
 *            1's in the binary expansion of n
 * Range:     u8 n <= 255, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;

    /* Count trailing zeros of n */
    idx_t tz = __builtin_ctzll((unsigned long long)n);

    /* Shift out those zeros to expose the last run of 1's */
    idx_t m = n >> tz;

    /* Length of that run = number of trailing zeros in ~m */
    idx_t len = __builtin_ctzll((unsigned long long)~m);

    /* Construct a mask of len ones and shift it back */
    seq_t mask = ((seq_t)1 << len) - 1;
    return mask << tz;
}
