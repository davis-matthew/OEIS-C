/*
 * A260390  Infinite palindromic word (a(1),a(2),a(3),...) with initial word
 *          w(1) = (1,0) and midword sequence (a(n)); see Comments.
 * https://oeis.org/A260390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th term of the infinite palindromic word generated
 *            from w(1) = (1,0) with midwords m(i) = a(i). The word is
 *            constructed by w(k) = w(k‑1) · a(k‑1) · reverse(w(k‑1)). The
 *            following implementation implements the recursive definition
 *            without using any external utilities
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base cases for the initial word w(1) = 10 */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)0;

    /* iterate until n falls to the base word */
    while (n > 2) {
        /* find the smallest palindrome length >= n */
        idx_t len = 2;          /* L(1) */
        idx_t k   = 1;          /* current level k */
        while (len < n) {
            len = 2 * len + 1;  /* L(k+1) = 2·L(k) + 1 */
            k++;
        }

        idx_t prev_len = (len - 1) / 2;   /* L(k‑1) */
        idx_t midpos  = prev_len + 1;     /* position of the middle symbol */

        if (n == midpos) {
            /* middle symbol is a(k‑1), so recurse with that index */
            return a(k - 1);
        }

        /* n lies in the suffix; reflect it to the corresponding
           position in the prefix */
        n = 2 * prev_len + 2 - n;  /* n' = len + 1 - n with len = 2·prev_len + 1 */
    }

    /* n is either 1 or 2 when the loop exits */
    return (n == 1) ? (seq_t)1 : (seq_t)0;
}
