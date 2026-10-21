/*
 * A277653  Number of n X 2 0..2 arrays with every element equal to some
 *          element at offset (-1,0) (-1,1) (0,-1) (0,1) or (1,0) both plus 1
 *          mod 3 and minus 1 mod 3, with new values introduced in order 0..2.
 * https://oeis.org/A277653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 20, u64 n <= 38, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
 *  Formula: a(n) = 3*a(n-1) + 2*a(n-2) - 2*a(n-4)   (for n ≥ 5)
 *  Initial values: a(1)=0, a(2)=0, a(3)=1, a(4)=2
 *  This recurrence generates OEIS A277653.
 */

seq_t a(idx_t n)
{
    /* Return the pre‑computed small values directly. */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)0;
    if (n == 3) return (seq_t)1;
    if (n == 4) return (seq_t)2;

    /* For n ≥ 5, iterate using the recurrence. */
    seq_t a1 = (seq_t)0;   /* a(n-4)   */
    seq_t a2 = (seq_t)0;   /* a(n-3)   */
    seq_t a3 = (seq_t)1;   /* a(n-2)   */
    seq_t a4 = (seq_t)2;   /* a(n-1)   */

    for (idx_t k = 5; k <= n; ++k) {
        seq_t next = ((seq_t)3) * a4
                  + ((seq_t)2) * a3
                  - ((seq_t)2) * a1;

        /* Shift the state forward for the next iteration. */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }
    return a4;
}
