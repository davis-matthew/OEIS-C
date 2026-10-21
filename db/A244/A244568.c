/*
 * A244568  Smallest value s such that n is in the trajectory of the 'Reverse
 *          and Add!' sequence starting with s.
 * https://oeis.org/A244568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 9389, u32 n <= 9389, u64 n <= 9389,
 *            u128 n <= 9389, big any n
 * Helpers:   rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation by brute‑force search of forward Reverse‑and‑Add trajectories up to n */
#include <stdint.h>
#include "../utils/rev.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* a(0) = 0 */

    for (idx_t s = 0; s <= n; ++s)
    {
        if (s == 0) continue;  /* 0 only reaches 0 */

        seq_t cur = (seq_t)s;

        if (cur == (seq_t)n) return (seq_t)s;

        while (cur < (seq_t)n)
        {
            idx_t revi = rev((idx_t)cur);
            cur = cur + (seq_t)revi;
            if (cur == (seq_t)n) return (seq_t)s;
        }
    }

    /* fallback – should not be reached */
    return (seq_t)n;
}
