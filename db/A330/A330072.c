/*
 * A330072  a(n) is the sum of all integers whose binary representation is
 *          contained in the binary representation of n (with multiplicity).
 * https://oeis.org/A330072
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of all integers whose binary representation is
 *            contained in the binary representation of n (with multiplicity)
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for any n >= 0.
   Complexity: O(b²) where b is the bit length of n (≤64). */
seq_t a(idx_t n)
{
    /* Fast path for zero. */
    if (n == 0)
        return 0;

    /* Determine the bit length of n. */
    idx_t bitlen = 0;
    idx_t tmp = n;
    while (tmp != 0) {
        ++bitlen;
        tmp >>= 1;
    }

    seq_t sum = 0;

    /* Iterate over all start/end positions of contiguous substrings. */
    for (idx_t start = 0; start < bitlen; ++start) {
        for (idx_t end = start; end < bitlen; ++end) {
            /* Length of the current substring. */
            idx_t len = end - start + 1;

            /* Position of the least‑significant bit of the substring. */
            idx_t shift = bitlen - 1 - end;

            /* Extract the substring value. */
            seq_t val = (seq_t)(n >> shift);

            /* Build a mask for the substring length. */
            seq_t mask;
            if (len == sizeof(seq_t) * 8)
                mask = ~((seq_t)0);
            else
                mask = (((seq_t)1 << len) - 1);

            val &= mask;
            sum += val;
        }
    }

    return sum;
}
