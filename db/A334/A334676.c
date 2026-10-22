/*
 * A334676  a(n) is the least number that can be reached starting from n and
 *          iterating the nondeterministic map x -> x/d where d is a nonzero
 *          digit of x dividing x.
 * https://oeis.org/A334676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the least number reachable from n by repeatedly dividing
 *            by a non‑zero digit of the current number that divides it
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;          /* current number */
    seq_t best = N;              /* start with itself */

    /* collect distinct non‑zero digits */
    bool seen[10] = {0};
    seq_t t = N;
    while (t > 0) {
        uint8_t d = (uint8_t)(t % 10);
        if (d != 0 && !seen[d]) {
            seen[d] = true;
            if (d != 1 && (N % d) == 0) {
                /* recursive call on the reduced number */
                seq_t cand = a((idx_t)(N / d));
                if (cand < best)
                    best = cand;
            }
        }
        t /= 10;
    }
    return best;
}
