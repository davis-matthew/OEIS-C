/*
 * A339495  Row sums of A339494.
 * https://oeis.org/A339495
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 32, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1)+a(n-2)+a(n-3)+a(n-4)+a(n-5) for n>=9,
   initial values:
   a(1)=1, a(2)=3, a(3)=9, a(4)=19, a(5)=37, a(6)=73,
   a(7)=144, a(8)=283 */
seq_t a(idx_t n)
{
    /* Handle the first eight terms directly */
    if (n <= 8) {
        static const seq_t init[8] = {
            (seq_t)1, (seq_t)3, (seq_t)9,
            (seq_t)19, (seq_t)37, (seq_t)73,
            (seq_t)144, (seq_t)283
        };
        return init[n - 1];
    }

    /* Set up a sliding window of the last five terms:
       prev[0] = a(n-5), prev[1] = a(n-4), ..., prev[4] = a(n-1)   */
    seq_t prev[5];
    prev[0] = (seq_t)19; /* a(4) */
    prev[1] = (seq_t)37; /* a(5) */
    prev[2] = (seq_t)73; /* a(6) */
    prev[3] = (seq_t)144;/* a(7) */
    prev[4] = (seq_t)283;/* a(8) */

    seq_t curr = 0; /* will hold a(n) */
    idx_t i;

    for (i = 9; i <= n; ++i) {
        curr = prev[0] + prev[1] + prev[2] + prev[3] + prev[4];
        /* shift the window */
        prev[0] = prev[1];
        prev[1] = prev[2];
        prev[2] = prev[3];
        prev[3] = prev[4];
        prev[4] = curr;
    }
    return curr;
}
