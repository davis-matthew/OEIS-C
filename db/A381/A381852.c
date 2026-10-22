/*
 * A381852  In the binary expansion of n (without leading zeros): complement
 *          the bits strictly to the right of the leftmost zero digit, if any.
 * https://oeis.org/A381852
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A381852: Complement bits to the right of the leftmost 0 in the binary expansion of n.
 *  Formula: a(n) = n                                   if n = 0 or all bits of n are 1,
 *           a(n) = n XOR (2^p – 1)                     otherwise,
 *           where p is the position (0‑based from LSB) of the most significant 0 bit in n.
 */

seq_t a(idx_t n)
{
    if (n == 0)          /* special case: binary representation is "0" */
        return (seq_t)0;

    /* Find floor(log2(n)).  Required only once per call. */
    unsigned int k = 0;
    idx_t t = n;
    while (t > 1) {      /* t >>= 1 will be zero at most 63 iterations */
        t >>= 1;
        ++k;
    }

    /* Scan from the most significant bit downwards. */
    for (int i = (int)k; i >= 0; --i) {
        if (((n >> i) & 1ULL) == 0) {   /* leftmost zero found */
            idx_t mask_full = ((idx_t)1 << i) - 1;  /* bits strictly to the right of i */
            seq_t mask = (seq_t)mask_full;
            return (seq_t)n ^ mask;    /* complement those bits */
        }
    }

    /* All bits are 1 – no zero found (n = 2^k – 1). */
    return (seq_t)n;
}
