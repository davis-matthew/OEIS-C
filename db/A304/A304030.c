/*
 * A304030  a(n) is the number of steps at which the Collatz ('3x+1')
 *          trajectory of n crosses its initial value, or -1 if the number of
 *          crossings is infinite.
 * https://oeis.org/A304030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 8958, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computes a(n) as the number of times the Collatz trajectory of n
   crosses its initial value (upward to downward or vice versa).  The
   function iterates the standard Collatz map until reaching 1, keeping
   track of whether the current value is above or below n and counting
   sign changes.  If the trajectory resulted in an infinite cycle that
   crosses n endlessly, the function would run forever; this situation
   does not occur under the Collatz conjecture, which is assumed.
*/
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* guard against out‑of‑range input */

    seq_t cur = (seq_t)n;          /* current value in the trajectory   */
    seq_t crossings = 0;          /* number of crossings               */
    int sign_prev = 0;            /* sign of previous value relative to n */

    seq_t n_s = (seq_t)n;          /* cast once for comparisons         */

    /* iterate until the trajectory reaches 1 */
    while (cur != 1)
    {
        /* compute next Collatz step */
        if ((cur & 1) == 0)               /* even case */
            cur = cur >> 1;
        else                              /* odd case 3x+1 */
            cur = cur * 3 + 1;

        /* determine sign relative to starting value n */
        int sign_cur;
        if (cur > n_s)      sign_cur = 1;
        else if (cur < n_s) sign_cur = -1;
        else                sign_cur = 0;   /* cur == n_s (rare) */

        /* count a crossing when the sign changes from non‑zero to
           opposite non‑zero sign (ignore initial state) */
        if (sign_prev != 0 && sign_cur != 0 && sign_cur != sign_prev)
            crossings++;

        sign_prev = sign_cur;          /* update previous sign */
    }

    return crossings;
}
