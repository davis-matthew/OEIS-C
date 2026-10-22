/*
 * A387905  a(n) = Sum_{k=0..n} binomial(4*n+2,5*k).
 * https://oeis.org/A387905
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A387905: a(n) = Sum_{k=0..n} binomial(4*n+2,5*k)
 *
 * The recurrence (valid for n > 4) is:
 *    a(n) = 5*a(n-1) + 130*a(n-2) + 740*a(n-3) - 65*a(n-4) + 16*a(n-5)
 *
 * Initial values for the recurrence (offset 0):
 *    a(0)=1, a(1)=7, a(2)=254, a(3)=3004, a(4)=53143
 */

seq_t a(idx_t n)
{
    /* Return the initial terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)7;
    if (n == 2) return (seq_t)254;
    if (n == 3) return (seq_t)3004;
    if (n == 4) return (seq_t)53143;

    /* arr[0] = a(n-5), arr[1] = a(n-4), arr[2] = a(n-3),
       arr[3] = a(n-2), arr[4] = a(n-1) for the current n in the loop. */
    seq_t arr[5];
    arr[0] = (seq_t)1;      /* a(0)   */
    arr[1] = (seq_t)7;      /* a(1)   */
    arr[2] = (seq_t)254;    /* a(2)   */
    arr[3] = (seq_t)3004;   /* a(3)   */
    arr[4] = (seq_t)53143;  /* a(4)   */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t cur = (seq_t)5   * arr[4]   /* 5*a(n-1)   */
                  + (seq_t)130 * arr[3]   /* 130*a(n-2) */
                  + (seq_t)740 * arr[2]   /* 740*a(n-3) */
                  - (seq_t)65  * arr[1]   /* -65*a(n-4) */
                  + (seq_t)16  * arr[0];  /* 16*a(n-5)  */

        /* Shift window for next iteration. */
        arr[0] = arr[1];
        arr[1] = arr[2];
        arr[2] = arr[3];
        arr[3] = arr[4];
        arr[4] = cur;
    }

    return arr[4];
}
