/*
 * A284787  Even numbers representable in at least two ways as the sum of two
 *          odd composites.
 * https://oeis.org/A284787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A284787: Even numbers representable in at least two ways as the sum of two odd composites.
   For n >= 11, a(n) = 2*n + 48.
   The first 10 terms are listed directly. */
static const seq_t _a_small[10] = {
    30, 36, 42, 48, 50,
    54, 58, 60, 64, 66
};

seq_t a(idx_t n)
{
    /* Handle the offset: a(1) = 30. */
    if (n == 0)
        return (seq_t)0;           /* Undefined for n < 1, return 0. */
    if (n <= 10)
        return _a_small[n - 1];
    /* For n >= 11 the formula applies. */
    return (seq_t)n * (seq_t)2 + (seq_t)48;
}
