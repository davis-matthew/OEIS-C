/*
 * A308184  S_oo, where S_1 = bc, S_n = S_{n-1} a^n S_{n-1} for n > 1, over
 *          the alphabet {a,b,c} = {1,2,3}.
 * https://oeis.org/A308184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 107, u16 n <= 107, u32 n <= 107, u64 n <= 107,
 *            u128 n <= 107, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A308184
   Infinite word defined by
        S1 = "bc"          (b = 2, c = 3)
        Sn = Sn-1 a^n Sn-1  for n > 1
   a(n) is the nth symbol (1‑based) of the limit word.
   The function iteratively finds the position of index n
   inside the smallest Sn that contains it. */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n = 0 – safe fallback */

    /* Lengths of S_k, stored in idx_t so we can hold large values. */
    idx_t lens[128];
    int levels = 0;                /* current level: lens[0] = |S1| = 2 */
    lens[0] = 2;

    /* Grow until the current level’s length covers n. */
    while (lens[levels] < n) {
        levels++;
        lens[levels] = lens[levels - 1] * 2 + (idx_t)(levels + 1);
    }

    /* Walk down from that level to find the character. */
    while (levels > 0) {
        idx_t prev_len = lens[levels - 1];          /* |S_{k-1}| */
        idx_t middle   = (idx_t)(levels + 1);       /* length of a^k */

        if (n <= prev_len) {                       /* left part */
            levels--;
            continue;
        } else if (n > prev_len + middle) {        /* right part */
            n -= prev_len + middle;
            levels--;
            continue;
        } else {                                   /* middle block of a's */
            return 1;                              /* 'a' = 1 */
        }
    }

    /* At level 0 the word is "bc". */
    return (n == 1) ? 2 : 3;
}
