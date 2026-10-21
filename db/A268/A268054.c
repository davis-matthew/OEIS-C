/*
 * A268054  Number of n X 6 0..2 arrays with new values introduced in each row
 *          and column in sequential order starting with zero.
 * https://oeis.org/A268054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 10, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Recurrence: a(n) = 184*a(n-1) - 8533*a(n-2) + 122786*a(n-3)
 *              - 563036*a(n-4) + 700280*a(n-5)
 * Initial values (offset 1): a(1)=1, a(2)=32, a(3)=2094,
 *                           a(4)=200818, a(5)=22501266
 */
seq_t a(idx_t n)
{
    /* Handle indices below the offset – behaviour not specified */
    if (n == 0)
        return (seq_t)0;

    /* Return the five initial values directly. */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)32;
    if (n == 3) return (seq_t)2094;
    if (n == 4) return (seq_t)200818;
    if (n == 5) return (seq_t)22501266;

    /* Compute subsequent terms using the linear recurrence.  */
    seq_t a1 = (seq_t)1;           /* a(n-5)  */
    seq_t a2 = (seq_t)32;          /* a(n-4)  */
    seq_t a3 = (seq_t)2094;        /* a(n-3)  */
    seq_t a4 = (seq_t)200818;      /* a(n-2)  */
    seq_t a5 = (seq_t)22501266;    /* a(n-1)  */
    seq_t an   = a5;               /* placeholder for the current term */

    for (idx_t k = 6; k <= n; ++k) {
        an = (seq_t)184 * a5
            - (seq_t)8533 * a4
            + (seq_t)122786 * a3
            - (seq_t)563036 * a2
            + (seq_t)700280 * a1;
        a1 = a2; a2 = a3; a3 = a4; a4 = a5; a5 = an;
    }
    return an;
}
