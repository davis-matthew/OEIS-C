/*
 * A398665  Lower (1/2, 2) midsequence of triangular numbers (A000217) and
 *          tetrahedral numbers (A000330); see Comments.
 * https://oeis.org/A398665
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 7*a(n-2) + 8*a(n-3) - 7*a(n-4) + 4*a(n-5) -
 *            a(n-6); a(0)=0, a(1)=2, a(2)=11, a(3)=31, a(4)=65, a(5)=117
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the initial seven terms directly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)11;
    if (n == 3) return (seq_t)31;
    if (n == 4) return (seq_t)65;
    if (n == 5) return (seq_t)117;

    /* iterate the linear recurrence from n = 6 up to the requested n */
    seq_t a0 = (seq_t)0;   /* a(n-6)  */
    seq_t a1 = (seq_t)2;   /* a(n-5)  */
    seq_t a2 = (seq_t)11;  /* a(n-4)  */
    seq_t a3 = (seq_t)31;  /* a(n-3)  */
    seq_t a4 = (seq_t)65;  /* a(n-2)  */
    seq_t a5 = (seq_t)117; /* a(n-1)  */
    seq_t next;
    idx_t i;

    for (i = 6; i <= n; ++i) {
        next = 4*a5 - 7*a4 + 8*a3 - 7*a2 + 4*a1 - a0;
        a0 = a1; a1 = a2; a2 = a3; a3 = a4; a4 = a5; a5 = next;
    }
    return a5;
}
