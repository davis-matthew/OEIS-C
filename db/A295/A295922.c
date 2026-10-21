/*
 * A295922  Number of (not necessarily maximal) cliques in the n-halved cube
 *          graph.
 * https://oeis.org/A295922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 13*a(n-1) - 68*a(n-2) + 184*a(n-3) - 272*a(n-4)
 *               + 208*a(n-5) - 64*a(n-6).
 * Initial terms: a(1)=2, a(2)=4, a(3)=16, a(4)=81, a(5)=393, a(6)=1777.
 * The function works for any n ≥ 1 and uses only integer arithmetic. */

seq_t a(idx_t n)
{
    /* Handle the first few terms directly. */
    if (n == 0) return 0;                    /* Safety for n < 1. */
    if (n <= 6) {
        static const seq_t init[7] = {0, 2, 4, 16, 81, 393, 1777};
        return init[n];
    }

    /* Ring buffer holding the last six values. */
    seq_t a1 = 2;    /* a(1) */
    seq_t a2 = 4;    /* a(2) */
    seq_t a3 = 16;   /* a(3) */
    seq_t a4 = 81;   /* a(4) */
    seq_t a5 = 393;  /* a(5) */
    seq_t a6 = 1777; /* a(6) */

    for (idx_t i = 7; i <= n; ++i) {
        /* Compute the next term using the recurrence. */
        seq_t sum_pos = 13 * a6 + 184 * a4 + 208 * a2;
        seq_t sum_neg = 68 * a5 + 272 * a3 + 64 * a1;
        seq_t next    = sum_pos - sum_neg;

        /* Shift the ring buffer. */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = a6;
        a6 = next;
    }

    return a6;
}
