/*
 * A341242  Numbers whose binary representation encodes a subset S of the
 *          natural numbers such that the XOR of the binary representations of
 *          all s in S gives 0.
 * https://oeis.org/A341242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th non‑negative integer whose set‑bit positions
 *            XOR to 0
 * Range:     u8 n <= 32, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;   /* undefined for n < 1, return 0 */

    idx_t count = 0;
    seq_t val = 0;

    while (count < n) {
        /* Check if current val satisfies the XOR condition. */
        idx_t xor_sum = 0;
        seq_t tmp = val;
        idx_t pos = 0;
        while (tmp != (seq_t)0) {
            if ((tmp & (seq_t)1) != (seq_t)0)
                xor_sum ^= pos;
            tmp >>= 1;
            pos++;
        }
        if (xor_sum == 0) {
            count++;
            if (count == n)
                return val;
        }
        val++;   /* next candidate */
    }
    return (seq_t)0;   /* unreachable, keeps compiler happy */
}
