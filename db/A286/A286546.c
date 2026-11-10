/*
 * A286546  a(n) = A006068(n) - n.
 * https://oeis.org/A286546
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = A006068(n) - n, where A006068(n) = n XOR floor(n/2) XOR
 *            floor(n/4) XOR
 * Range:     i8 n <= 254, i16 n <= 16383, i32 n <= 16383, i64 n <= 16383,
 *            i128 n <= 16383, big any n
 * Validated: standard, pass, 96 terms through n = 9506 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t A006068_a(idx_t n)
{
    seq_t result = (seq_t)n;
    seq_t shifted = (seq_t)n;

    while (shifted > 0) {
        shifted >>= 1;
        result ^= shifted;
    }

    return result;
}

seq_t a(idx_t n)
{
    return A006068_a(n) - (seq_t)n;
}
