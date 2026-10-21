/*
 * A295979  Number of n X 2 0..1 arrays with each 1 adjacent to 2 or 3
 *          king-move neighboring 1s.
 * https://oeis.org/A295979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) + 9*a(n-3) - 10*a(n-4) - 4*a(n-5) -
 *            4*a(n-6) with initial values: a(1)=1, a(2)=6, a(3)=15, a(4)=28,
 *            a(5)=90, a(6)=281
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 42, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return directly for known small indices to avoid unnecessary work */
    if (n == 0) return 0;         /* value below the offset – undefined */
    if (n == 1) return 1;
    if (n == 2) return 6;
    if (n == 3) return 15;
    if (n == 4) return 28;
    if (n == 5) return 90;
    if (n == 6) return 281;

    /* arr[0]..arr[5] hold a(n-1) .. a(n-6) for the current step */
    seq_t arr[6];
    arr[0] = 281u;   /* a(6)  */
    arr[1] = 90u;    /* a(5)  */
    arr[2] = 28u;    /* a(4)  */
    arr[3] = 15u;    /* a(3)  */
    arr[4] = 6u;     /* a(2)  */
    arr[5] = 1u;     /* a(1)  */

    for (idx_t i = 7; i <= n; ++i)
    {
        /* Compute the next term using the recurrence with
           a positive intermediate value to avoid underflow on subtraction. */
        seq_t pos = 3u * arr[0] + 9u * arr[2];
        seq_t neg = 2u * arr[1] + 10u * arr[3] + 4u * arr[4] + 4u * arr[5];
        seq_t nxt = pos - neg;

        /* Shift the window for the next iteration */
        arr[5] = arr[4];
        arr[4] = arr[3];
        arr[3] = arr[2];
        arr[2] = arr[1];
        arr[1] = arr[0];
        arr[0] = nxt;
    }
    return arr[0];
}
