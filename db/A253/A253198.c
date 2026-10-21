/*
 * A253198  a(n) = a(n-1) + a(n-2) - (-1)^(a(n-1) + a(n-2)) with a(0)=0,
 *          a(1)=1.
 * https://oeis.org/A253198
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) - (-1)^(a(n-1)+a(n-2)) Initial values:
 *            a(0) = 0, a(1) = 1 Recurrence uses only addition, subtraction,
 *            and a test for parity
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the trivial small indices. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* Keep the two most recent terms only. */
    seq_t a0 = (seq_t)0;   /* a(n-2) */
    seq_t a1 = (seq_t)1;   /* a(n-1) */
    seq_t an;              /* a(n) */

    for (idx_t i = 2; i <= n; ++i) {
        /* sum of the two previous terms */
        seq_t sum = a0 + a1;

        /* (-1)^(sum) is 1 if sum even, -1 if sum odd.
           Therefore: subtract 1 if sum even, otherwise add 1. */
        if ((sum & (seq_t)1) == 0) {          /* sum is even */
            an = sum - (seq_t)1;
        } else {                              /* sum is odd */
            an = sum + (seq_t)1;
        }

        /* Roll over for next iteration. */
        a0 = a1;
        a1 = an;
    }

    return a1;   /* after loop a1 holds a(n) */
}
