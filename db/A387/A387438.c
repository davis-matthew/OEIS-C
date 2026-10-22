/*
 * A387438  Number of Hamiltonian paths in the n-Cameron graph.
 * https://oeis.org/A387438
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 30, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 5*a(n-1) - 5*a(n-2) + a(n-3) - a(n-4) + a(n-5) - a(n-6) + a(n-7),
   initial values: a(0)=2, a(1)=106, a(2)=614, a(3)=2658, a(4)=10406,
   a(5)=39298, a(6)=146618 */

seq_t a(idx_t n)
{
    /* Handle the first seven terms directly. */
    switch (n) {
        case 0u: return 2;
        case 1u: return 106;
        case 2u: return 614;
        case 3u: return 2658;
        case 4u: return 10406;
        case 5u: return 39298;
        case 6u: return 146618;
    }

    /* Keep the last seven computed values. */
    seq_t a0 = 2;
    seq_t a1 = 106;
    seq_t a2 = 614;
    seq_t a3 = 2658;
    seq_t a4 = 10406;
    seq_t a5 = 39298;
    seq_t a6 = 146618;
    seq_t next;

    for (idx_t i = 7; i <= n; ++i) {
        next = (seq_t)5 * a6 - (seq_t)5 * a5 + a4 - a3 + a2 - a1 + a0;
        /* Shift the sliding window. */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = a6;
        a6 = next;
    }

    return a6;
}
