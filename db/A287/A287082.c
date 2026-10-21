/*
 * A287082  Number of edge covers on the n-web graph.
 * https://oeis.org/A287082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3 - sqrt(17))^n + (3 + sqrt(17))^n Recurrence: a(1) = 6,
 *            a(2) = 52, a(n) = 6*a(n-1) + 8*a(n-2) for n > 2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two values directly. */
    if (n == 1) {
        return (seq_t)6;
    }
    if (n == 2) {
        return (seq_t)52;
    }

    /* Constants expressed as seq_t to work with all supported types. */
    const seq_t six  = (seq_t)6;
    const seq_t eight = (seq_t)8;

    /* a1 holds a(n-2), a2 holds a(n-1). */
    seq_t a1 = (seq_t)6;   /* a(1) */
    seq_t a2 = (seq_t)52;  /* a(2) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t cur = six * a2 + eight * a1; /* a(i) = 6*a(i-1) + 8*a(i-2) */
        a1 = a2;
        a2 = cur;
    }

    return a2;
}
