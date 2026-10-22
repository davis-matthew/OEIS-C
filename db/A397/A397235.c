/*
 * A397235  Expansion of 1/(1 - 2*x - x^2 + x^3 - x^4).
 * https://oeis.org/A397235
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - a(n-3) + a(n-4); a(0)=1, a(1)=2,
 *            a(2)=5, a(3)=11
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 26, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Predefined initial terms */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)11;

    /* Rolling variables for the recurrence */
    seq_t a0 = (seq_t)1;   /* a(n-4) */
    seq_t a1 = (seq_t)2;   /* a(n-3) */
    seq_t a2 = (seq_t)5;   /* a(n-2) */
    seq_t a3 = (seq_t)11;  /* a(n-1) */
    seq_t next;

    for (idx_t i = 4; i <= n; ++i) {
        next = (seq_t)2 * a3 + a2 - a1 + a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
