/*
 * A355549  Number of positive integers k with 1 < k < n such that n occurs in
 *          the Reverse-and-Multiply trajectory of k.
 * https://oeis.org/A355549
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of k with 1 < k < n whose Reverse‑and‑Multiply
   trajectory contains n.  For each k we iterate
   cur := cur * rev(cur) until cur > n or cur == n. 
   rev(x) reverses the decimal digits of x (x fits in idx_t). */

#include <stdint.h>                  /* for uint64_t; idx_t is defined elsewhere */

/* Internal helper: reverse the decimal digits of an idx_t value. */
static inline idx_t rev_uint64(idx_t x)
{
    idx_t r = 0;
    while (x > 0) {
        r = r * 10 + (x % 10);
        x /= 10;
    }
    return r;
}

seq_t a(idx_t n)
{
    seq_t count = 0;

    /* For n = 0,1,2 there is no k with 1<k<n. */
    if (n < 3) {
        return 0;
    }

    seq_t n_val = (seq_t)n;   /* cast once for repeated use */

    for (idx_t k = 2; k < n; ++k) {
        seq_t cur = (seq_t)k;

        while (1) {
            if (cur == n_val) {          /* n is reached on this trajectory */
                ++count;
                break;
            }

            idx_t rev = rev_uint64((idx_t)cur);
            if (rev <= 1) {
                /* rev(cur) == 1 would cause an infinite loop (cur==1) */
                break;
            }

            seq_t r = (seq_t)rev;        /* safe cast: rev <= n < 10000 */
            if (cur > n_val / r) {       /* would exceed n or overflow seq_t */
                break;
            }

            cur = cur * r;
        }
    }

    return count;
}
