/*
 * A281982  Number of n X 2 0..1 arrays with no element unequal to more than
 *          four of its king-move neighbors, with the exception of exactly one
 *          element, and with new values introduced in order 0 sequentially
 *          upwards.
 * https://oeis.org/A281982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence: 
   a(n) = 8*a(n-1) - 20*a(n-2) + 24*a(n-3) - 36*a(n-4) + 16*a(n-5) - 16*a(n-6)
   with initial values a(1)=0, a(2)=0, a(3)=2, a(4)=16, a(5)=88, a(6)=432 */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0; /* undefined for n < 1, but safe */
    /* Initial terms – index 0 unused */
    static const seq_t init[7] = {0, 0, 0, 2, 16, 88, 432};

    if (n <= 6) return init[n];

    /* Keep the last six values a(n-6) .. a(n-1) */
    seq_t buf[6] = {init[1], init[2], init[3], init[4], init[5], init[6]};
    seq_t next = (seq_t)0;

    for (idx_t i = 7; i <= n; ++i) {
        next = ((seq_t)8)*buf[5]
             - ((seq_t)20)*buf[4]
             + ((seq_t)24)*buf[3]
             - ((seq_t)36)*buf[2]
             + ((seq_t)16)*buf[1]
             - ((seq_t)16)*buf[0];

        /* roll the buffer: drop a(n-6), shift others left, insert new value */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = buf[5];
        buf[5] = next;
    }
    return next;
}
