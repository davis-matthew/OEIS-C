/*
 * A297860  Number of n X 2 0..1 arrays with every element equal to 2, 3 or 4
 *          king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A297860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-2) + 4*a(n-3) + 2*a(n-4) for n >= 5, with initial
 *            values: a(1)=0, a(2)=1, a(3)=2, a(4)=5
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 29, u64 n <= 56, u128 n <= 110,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the small indices explicitly. */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)2;
    if (n == 4) return (seq_t)5;

    /* Iteratively compute the sequence up to n. */
    seq_t f0 = (seq_t)0; /* a(n-4) */
    seq_t f1 = (seq_t)1; /* a(n-3) */
    seq_t f2 = (seq_t)2; /* a(n-2) */
    seq_t f3 = (seq_t)5; /* a(n-1) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t next = (seq_t)3 * f2 + (seq_t)4 * f1 + (seq_t)2 * f0;
        f0 = f1;
        f1 = f2;
        f2 = f3;
        f3 = next;
    }

    return f3;
}
