/*
 * A297883  Number of n X 2 0..1 arrays with every element equal to 0, 1, 2 or
 *          4 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A297883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) + 8*a(n-3) - 20*a(n-4) + 8*a(n-5) for
 *            n>6 Initial values: a(1)=2, a(2)=7, a(3)=13, a(4)=29, a(5)=69,
 *            a(6)=137
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 27, u64 n <= 55, u128 n <= 110,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Guard against n < 1 */
    if (n < 1) return (seq_t)0;

    /* Handle the first six terms directly */
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)7;
    if (n == 3) return (seq_t)13;
    if (n == 4) return (seq_t)29;
    if (n == 5) return (seq_t)69;
    if (n == 6) return (seq_t)137;

    /* Initialise the last five values (a(n-5) ... a(n)) */
    seq_t a2 = (seq_t)2;
    seq_t a3 = (seq_t)7;
    seq_t a4 = (seq_t)13;
    seq_t a5 = (seq_t)29;
    seq_t a6 = (seq_t)69;  /* These are a(n-5) .. a(n-1) for n=7 */
    seq_t a7 = (seq_t)137; /* a(6) is a(n-0) for n=7 */

    /* Iterate from 7 up to the requested n */
    for (idx_t i = 7; i <= n; ++i)
    {
        /* Compute next term using the recurrence */
        seq_t next = 3 * a7          /* 3*a(n-1) */
                  - 2 * a6          /* 2*a(n-2) */
                  + 8 * a5          /* 8*a(n-3) */
                  - 20 * a4         /* 20*a(n-4) */
                  + 8 * a3;         /* 8*a(n-5) */

        /* Shift the window:
           a(n-5) <- a(n-4)
           a(n-4) <- a(n-3)
           a(n-3) <- a(n-2)
           a(n-2) <- a(n-1)
           a(n-1) <- a(n)
           a(n-0) <- next   */
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = a6;
        a6 = a7;
        a7 = next;
    }

    /* After the loop, a7 holds a(n) */
    return a7;
}
