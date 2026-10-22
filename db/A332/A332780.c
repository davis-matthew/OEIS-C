/*
 * A332780  a(n) = (a(n-1) XOR a(n-3)) + 1, a(0) = a(1) = a(2) = 0.
 * https://oeis.org/A332780
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = a(1) = a(2) = 0
   a(n) = (a(n-1) XOR a(n-3)) + 1   for n ≥ 3
   This implementation computes the term iteratively using only three
   rolling variables, so it works for any arbitrary-precision seq_t
   without dynamic allocation. */
seq_t a(idx_t n)
{
    /* Base cases */
    if (n < 3)
        return (seq_t)0;

    /* Rolling variables: a0 = a(i-3), a1 = a(i-2), a2 = a(i-1) */
    seq_t a0 = (seq_t)0;
    seq_t a1 = (seq_t)0;
    seq_t a2 = (seq_t)0;
    seq_t ai;               /* a(i) */

    for (idx_t i = 3; i <= n; ++i) {
        ai = (a2 ^ a0) + (seq_t)1;
        a0 = a1;
        a1 = a2;
        a2 = ai;
    }

    return a2;
}
