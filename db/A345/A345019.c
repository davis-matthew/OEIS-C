/*
 * A345019  Numbers whose last digit is refactorable.
 * https://oeis.org/A345019
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = 10*((n-1)/4) + [1,2,8,9][(n-1) mod 4]
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1. Return 0 for n == 0 as a safe
       fallback, though the sequence offset starts at 1. */
    if (n == (idx_t)0)
        return (seq_t)0;

    seq_t nseq = (seq_t)n;                     /* Convert to seq_t once.   */
    seq_t offset = (nseq - (seq_t)1) / (seq_t)4; /* floor((n-1)/4).         */

    /* Map the repeating pattern of last digits.  The index is
       (n-1) mod 4. */
    static const seq_t digit_map[4] = {
        (seq_t)1,
        (seq_t)2,
        (seq_t)8,
        (seq_t)9
    };
    unsigned idx = (unsigned)((nseq - (seq_t)1) % (seq_t)4);

    return offset * (seq_t)10 + digit_map[idx];
}
