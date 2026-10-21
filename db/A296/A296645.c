/*
 * A296645  Number of n X 2 0..1 arrays with each 1 adjacent to 0, 1 or 3
 *          king-move neighboring 1s.
 * https://oeis.org/A296645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 3*a(n-2) + 9*a(n-3) - 4*a(n-4) - 12*a(n-5) -
 *            4*a(n-6); initial values: a(1)=4, a(2)=12, a(3)=29, a(4)=85,
 *            a(5)=248, a(6)=664
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 42, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)12;
    if (n == 3) return (seq_t)29;
    if (n == 4) return (seq_t)85;
    if (n == 5) return (seq_t)248;
    if (n == 6) return (seq_t)664;

    seq_t a1 = (seq_t)4,   /* a(1) */
           a2 = (seq_t)12, /* a(2) */
           a3 = (seq_t)29, /* a(3) */
           a4 = (seq_t)85, /* a(4) */
           a5 = (seq_t)248,/* a(5) */
           a6 = (seq_t)664;/* a(6) */

    for (idx_t i = 7; i <= n; ++i) {
        seq_t left  = a6 + (seq_t)3 * a5 + (seq_t)9 * a4;
        seq_t right = (seq_t)4 * a3 + (seq_t)12 * a2 + (seq_t)4 * a1;
        seq_t next  = left - right;

        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = a6;
        a6 = next;
    }

    return a6;
}
