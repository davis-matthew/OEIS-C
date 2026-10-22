/*
 * A397113  Lower (2,-1) midsequence of (floor[n/2]^2) and (ceiling[n/2]^2);
 *          see Comments.
 * https://oeis.org/A397113
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-2) - 2*a(n-3) - a(n-4) + a(n-5); Initial
 *            values: a(0)=0, a(1)=-1, a(2)=1, a(3)=-2, a(4)=4, a(5)=-1;
 * Range:     i8 n <= 23, i16 n <= 60, i32 n <= 60, i64 n <= 60, i128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* seq_t and idx_t are provided by the build environment. */
seq_t a(idx_t n)
{
    /* Handle the initial terms directly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)-1;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)-2;
    if (n == 4) return (seq_t)4;
    if (n == 5) return (seq_t)-1;

    /* For n >= 6, use the recurrence iteratively. */
    seq_t v0 = (seq_t)-1;   /* a(i-1)   for i=6  -> a5  */
    seq_t v1 = (seq_t)4;    /* a(i-2)   for i=6  -> a4  */
    seq_t v2 = (seq_t)-2;   /* a(i-3)   for i=6  -> a3  */
    seq_t v3 = (seq_t)1;    /* a(i-4)   for i=6  -> a2  */
    seq_t v4 = (seq_t)-1;   /* a(i-5)   for i=6  -> a1  */
    seq_t res = 0;

    for (idx_t i = 6; i <= n; ++i) {
        /* a(i) = a(i-1) + 2*a(i-2) - 2*a(i-3) - a(i-4) + a(i-5) */
        res = v0 + v1 * (seq_t)2 - v2 * (seq_t)2 - v3 + v4;

        /* Rotate the window for the next iteration. */
        v4 = v3;
        v3 = v2;
        v2 = v1;
        v1 = v0;
        v0 = res;
    }

    return res;  /* res now holds a(n) */
}
