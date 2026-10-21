/*
 * A260455  Infinite palindromic word (a(1),a(2),a(3),...) with initial word
 *          w(1) = 0 and midword sequence (1,null,1,null,1,null,...); see
 *          Comments.
 * https://oeis.org/A260455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth symbol of the infinite palindromic binary word
 *            constructed by: w(1) = "0" w(k+1) = w(k) + (k odd ? "1" : "") +
 *            reverse(w(k)) Thus a(n) = 1 - A260456(n). A '1' appears only at
 *            even construction levels (when k is even). This function finds
 *            the minimal level that contains position n...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* length of the words w(1..k) */
    idx_t len[65];
    idx_t k = 1;
    len[1] = 1;

    /* Build lengths until one exceeds or equals n */
    while (len[k] < n) {
        ++k;
        if ((k & 1) == 0)          /* even level – insert '1' */
            len[k] = 2 * len[k-1] + 1;
        else                       /* odd level – no middle symbol */
            len[k] = 2 * len[k-1];
    }

    /* Current position within the word (1‑based) */
    idx_t pos = n;

    /* Walk backwards from level k to 1, mapping the index */
    while (k > 1) {
        idx_t left = len[k-1];
        if ((k & 1) == 0) {                /* even level – middle is '1' */
            if (pos == left + 1)
                return (seq_t)1;            /* middle symbol */
            if (pos > left + 1)
                pos = 2 * left + 2 - pos;   /* map into left part */
        } else {                            /* odd level – no middle */
            if (pos > left)
                pos = 2 * left + 1 - pos;   /* map into left part */
        }
        --k;
    }

    /* At level 1 the only symbol is '0' */
    return (seq_t)0;
}
