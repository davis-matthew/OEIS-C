/*
 * A246036  Expansion of (1+4*x)/((1+2*x)*(1-4*x)).
 * https://oeis.org/A246036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 8*a(n-2), a(0) = 1, a(1) = 6
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial values directly. */
    if (n == 0ULL)
        return (seq_t)1;
    if (n == 1ULL)
        return (seq_t)6;

    /* Use a simple iterative recurrence.  All values fit in seq_t
       as long as a(n) itself fits, so intermediate arithmetic is
       safe on all supported integer types. */
    seq_t prev2 = (seq_t)1;      /* a(0) */
    seq_t prev1 = (seq_t)6;      /* a(1) */
    seq_t curr;

    for (idx_t i = 2ULL; i <= n; ++i) {
        curr = (seq_t)2 * prev1 + (seq_t)8 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }

    return prev1;
}
