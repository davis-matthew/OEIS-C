/*
 * A341755  Maximum number of consecutive integers starting at n such that any
 *          pair (k, k+1) among these shares at least one digit with its sum k
 *          + (k+1) = 2k + 1.
 * https://oeis.org/A341755
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 1140, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A341755:
 *  a(n) = maximum number of consecutive integers starting at n such that
 *  for every adjacent pair (k,k+1) the sum 2k+1 shares at least one digit
 *  with either k or k+1.  If no such pair exists, a(n)=0.  The length is
 *  counted as the number of integers in the chain.
 *
 * The algorithm simply tests successive pairs until a failure.
 */

#include <stdbool.h>

/* Return an 10‑bit mask of the decimal digits present in v.
 * Bit i is set iff digit i appears in v. */
static inline unsigned short digit_mask(seq_t v)
{
    unsigned short m = 0;
    do {
        unsigned int d = (unsigned int)(v % 10);
        m |= 1u << d;
        v = v / 10;
    } while (v);
    return m;
}

seq_t a(idx_t n)
{
    seq_t k = (seq_t)n;      /* current integer in the chain   */
    idx_t  successes = 0;    /* number of consecutive successful pairs */

    for (;;) {
        seq_t next = k + 1;                      /* k+1            */
        seq_t sum  = k * 2 + 1;                  /* 2k+1           */
        unsigned short mk   = digit_mask(k);
        unsigned short mnext = digit_mask(next);
        unsigned short msum  = digit_mask(sum);

        /* Check whether sum shares a digit with either k or k+1. */
        if (msum & (mk | mnext)) {
            ++successes;
            k = next;            /* continue with next integer */
        } else {
            break;               /* chain ends here           */
        }
    }

    if (successes == 0)
        return 0;                      /* failed on the first pair  */
    else
        return (seq_t)(successes + 1);  /* number of vertices        */
}
