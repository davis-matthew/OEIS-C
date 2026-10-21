/*
 * A260456  Infinite palindromic word (a(1),a(2),a(3),...) with initial word
 *          w(1) = 1 and midword sequence (0,null,0,null,0,null,...); see
 *          Comments.
 * https://oeis.org/A260456
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

/* Recurrence: w1 = "1".  
   For k ≥ 2,  w_k = w_{k-1} + m_{k-1} + reverse(w_{k-1}),  
   where m_{k-1} = "0" if k-1 is odd, otherwise the empty word.  
   The infinite word is the limit of w_k, and a(n) is the
   n‑th symbol (1‑based) of this word.
   Initial values: a(1)=1, a(2)=0, a(3)=1, … */

#include <stdint.h>
#include <limits.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* out of bounds – safety */

    /* Find the smallest k such that |w_k| ≥ n.
       Len is kept in unsigned __int128 to avoid overflow. */
    unsigned __int128 len = 1;            /* |w_1| = 1 */
    unsigned k = 1;                       /* current stage */

    while (len < (unsigned __int128)n) {
        ++k;
        unsigned s = (k % 2 == 0) ? 1u : 0u;  /* midword length */
        len = (len << 1) + s;                /* new length  = 2*len + s */
    }

    /* Descend back to w_1, transforming the position n accordingly. */
    while (k > 1) {
        unsigned s = (k % 2 == 0) ? 1u : 0u;      /* midword length at this step */
        unsigned __int128 prev_len = (len - s) >> 1;  /* |w_{k-1}| */

        if ((unsigned __int128)n <= prev_len) {    /* first half */
            len = prev_len;
            --k;
            continue;
        }

        if (s == 1u && (unsigned __int128)n == prev_len + 1) {
            return 0;                               /* middle '0' */
        }

        /* In the reverse part.  Compute new position in w_{k-1}. */
        unsigned __int128 offset128 = (unsigned __int128)n - (prev_len + s);
        idx_t offset = (idx_t)offset128;                    /* offset ≤ prev_len ≤ UINT64_MAX */
        n = (idx_t)(prev_len - offset128 + 1);             /* reverse mapping */
        len = prev_len;
        --k;
    }

    /* k == 1  ⇒  the only symbol is '1'. */
    return 1;
}
