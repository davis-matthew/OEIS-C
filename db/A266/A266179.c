/*
 * A266179  Binary representation of the n-th iteration of the "Rule 6"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266179
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 100ⁿ (n even), 110·100ⁿ⁻¹ (n odd)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_uint(seq_t base, idx_t exp) {
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if ((n & 1) == 0) {               /* even n */
        return pow_uint((seq_t)100, n);
    } else {                           /* odd n */
        /* n >= 1 because a(0) handled above */
        return pow_uint((seq_t)100, n - 1) * (seq_t)110;
    }
}
