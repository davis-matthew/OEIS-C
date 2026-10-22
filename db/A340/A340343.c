/*
 * A340343  a(n) = (48*n mod 604) + (70*n mod 426) + (46*n mod 47) + (93*n mod
 *          1766).
 * https://oeis.org/A340343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = (48*n mod 604)+(70*n mod 426)+(46*n mod 47)+(93*n mod
 *            1766)
 * Range:     u8 none, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;

    result += ((seq_t)n * (seq_t)48) % (seq_t)604;
    result += ((seq_t)n * (seq_t)70) % (seq_t)426;
    result += ((seq_t)n * (seq_t)46) % (seq_t)47;
    result += ((seq_t)n * (seq_t)93) % (seq_t)1766;

    return result;
}
