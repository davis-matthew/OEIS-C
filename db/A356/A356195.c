/*
 * A356195  The binary expansion of a(n) is obtained by applying the
 *          totalistic cellular automaton with rule 2*n to the binary
 *          expansion of n.
 * https://oeis.org/A356195
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) is the binary string obtained by applying the totalistic
 *  cellular automaton with rule number 2*n to the binary representation
 *  of n.  For each bit position k of n (starting from the least
 *  significant bit) we count the number w of 1‑bits in the suffix
 *  n[k..] and output the w‑th bit (0‑based) of the binary form of 2*n.
 */
seq_t a(idx_t n)
{
    /* Result accumulator */
    seq_t result = 0;

    /* Count total number of 1‑bits in n (maximum w at the first step). */
    idx_t ones = 0;
    idx_t tmp = n;
    while (tmp) {
        if (tmp & 1) ++ones;
        tmp >>= 1;
    }

    /* Rule number is 2 * n.  It is a seq_t, so we can shift it. */
    seq_t rule = (seq_t)n * 2;

    /* Scan the bits of n from LSB to MSB.  At each step we know
     * the number of remaining 1‑bits in the current suffix. */
    idx_t pos = 0;
    while ((n >> pos) != 0) {
        /* w is the current number of 1‑bits still to the left (including
         * the current bit).  The output bit is the w‑th bit of the rule. */
        idx_t w = ones;
        seq_t bit = (rule >> w) & 1;

        /* Add the bit to the result.  Use a 64‑bit shift for safety
         * to avoid shifting beyond the width of seq_t. */
        if (bit & 1) {
            result += (seq_t)((unsigned long long)1 << pos);
        }

        /* Update the count of remaining 1‑bits for the next position. */
        ones -= (n >> pos) & 1;
        ++pos;
    }

    return result;
}
