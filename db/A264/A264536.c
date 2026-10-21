/*
 * A264536  Number of (3+1) X (n+1) arrays of permutations of 0..n*4+3 with
 *          each element having directed index change -2,-2 -1,0 0,1 or 1,0.
 * https://oeis.org/A264536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 12*a(n-3) + 16*a(n-5) (empirical), a(1)=1 a(2)=4
 *            a(3)=13 a(4)=29 a(5)=89
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 43, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 6 use the known initial values. */
    if (n == 0) return 0;          /* safety for offset */
    if (n == 1) return 1;
    if (n == 2) return 4;
    if (n == 3) return 13;
    if (n == 4) return 29;
    if (n == 5) return 89;

    /* Use a sliding window of width 5 to avoid dynamic allocation and
       to work with both integer and arbitrary‑precision sequence types. */
    seq_t a1 = 1;   /* a(1)   */
    seq_t a2 = 4;   /* a(2)   */
    seq_t a3 = 13;  /* a(3)   */
    seq_t a4 = 29;  /* a(4)   */
    seq_t a5 = 89;  /* a(5)   */

    for (idx_t i = 6; i <= n; ++i) {
        seq_t next = a5 + (seq_t)12 * a3 + (seq_t)16 * a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = next;
    }
    return a5;
}
