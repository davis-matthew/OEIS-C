/*
 * A267531  Total number of OFF (white) cells after n iterations of the "Rule
 *          141" elementary cellular automaton starting with a single ON
 *          (black) cell.
 * https://oeis.org/A267531
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-3) + a(n-4) for n > 4 Initial values:
 *            a(0)=0, a(1)=2, a(2)=5, a(3)=9, a(4)=13
 * Range:     u8 n <= 27, u16 n <= 507, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)9;
    if (n == 4) return (seq_t)13;

    /* a1, a2, a3, a4 hold a(n-4), a(n-3), a(n-2), a(n-1) respectively. */
    seq_t a1 = (seq_t)2;   /* a(1) */
    seq_t a2 = (seq_t)5;   /* a(2) */
    seq_t a3 = (seq_t)9;   /* a(3) */
    seq_t a4 = (seq_t)13;  /* a(4) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t next = (seq_t)2 * a4 - (seq_t)2 * a2 + a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }
    return a4;
}
