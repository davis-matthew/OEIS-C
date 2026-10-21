/*
 * A260454  Infinite palindromic word (a(1),a(2),a(3),...) with initial word
 *          w(1) = (3,2,1) and midword sequence (a(n)); see Comments.
 * https://oeis.org/A260454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=3, a(2)=2, a(3)=1.
   For n>3 let p be the largest power of two ≤ n.
   If n==p then a(n)=a(log2(n)-1);
   else a(n)=a(p-(n-p)). */
seq_t a(idx_t n)
{
    /* base values */
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)1;

    while (n > 3) {
        /* largest power of two ≤ n */
        idx_t p = 1;
        while (p <= n >> 1) p <<= 1;

        if (n == p) {
            /* n is a power of two ≥ 4: reduce to log2(n)-1 */
            idx_t e = 0;
            idx_t tmp = n;
            while (tmp > 1) {
                tmp >>= 1;
                e++;
            }
            n = e - 1;   /* e ≥ 2 → n ≥ 1 */
            continue;
        }

        /* n lies in the right half; reflect to the left side */
        n = p - (n - p);
    }

    /* n is now 1, 2, or 3 */
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)2;
    return (seq_t)1;
}
