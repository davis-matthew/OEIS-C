/*
 * A242818  a(n) = 2^floor(n/2) n! [x^n] exp(x+x^2/4), where [x^n] f(x)
 *          denotes the coefficient of x^n in the expansion of f(x).
 * https://oeis.org/A242818
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^floor(n/2) * n! * [x^n] exp(x + x^2/4) Recurrence used:
 *            a(0)=1, a(1)=1, a(n+2) = 2^{even(n)} * a(n+1) + (n+1) * a(n)
 *            where even(n)=1 if n is even, otherwise 0
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 17, u64 n <= 30, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)1;

    seq_t a_prev = (seq_t)1;   /* a(0) */
    seq_t a_curr = (seq_t)1;   /* a(1) */

    /* Compute a(2) .. a(n) */
    for (idx_t i = 0u; i + 1u < n; ++i) {
        /* i corresponds to the lower index in the recurrence:
           we compute a(i+2) from a(i) and a(i+1). */
        seq_t k = (seq_t)(i + 1u);          /* (i+1) factor */

        /* part1 = 2^even(i) * a_curr  */
        seq_t part1 = ((i & 1u) == 0u) ? (a_curr + a_curr) : a_curr;

        /* part2 = (i+1) * a_prev        */
        seq_t part2 = k * a_prev;

        seq_t a_next = part1 + part2;

        a_prev = a_curr;
        a_curr = a_next;
    }

    return a_curr;
}
