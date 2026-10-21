/*
 * A284851  Fixed point of the morphism 0 -> 01, 1 -> 0100.
 * https://oeis.org/A284851
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

/* Sequence: fixed point of the morphism
   0 → 01, 1 → 0100 (offset 1).  a(n) is the n‑th symbol of the infinite word. */

#include <stdbool.h>
#include <stdint.h>

static seq_t get_symbol(idx_t pos, int depth, int sym,
                        const idx_t *len0, const idx_t *len1)
{
    if (depth == 0)
        return (seq_t)sym;

    if (sym == 0) {
        if (pos <= len0[depth - 1])
            return get_symbol(pos, depth - 1, 0, len0, len1);
        return get_symbol(pos - len0[depth - 1], depth - 1, 1, len0, len1);
    } else {               /* sym == 1 : pattern 0,1,0,0 */
        const idx_t L0 = len0[depth - 1];
        const idx_t L1 = len1[depth - 1];

        if (pos <= L0)
            return get_symbol(pos, depth - 1, 0, len0, len1);

        idx_t rem1 = pos - L0;              /* after first block */
        if (rem1 <= L1)
            return get_symbol(rem1, depth - 1, 1, len0, len1);

        idx_t rem2 = rem1 - L1;             /* after second block */
        if (rem2 <= L0)
            return get_symbol(rem2, depth - 1, 0, len0, len1);

        return get_symbol(rem2 - L0, depth - 1, 0, len0, len1);
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                       /* out of bounds */

    /* maximal depth needed is very small (~ 200 for 64‑bit indices) */
    const int MAX_DEPTH = 200;
    idx_t len0[MAX_DEPTH];
    idx_t len1[MAX_DEPTH];

    len0[0] = 1;
    len1[0] = 1;
    int depth = 0;

    /* compute lengths until len0[depth] ≥ n */
    while (len0[depth] < n && depth + 1 < MAX_DEPTH) {
        ++depth;
        idx_t a0 = len0[depth - 1];
        idx_t a1 = len1[depth - 1];

        /* len0[depth] = len0[depth-1] + len1[depth-1] */
        if (a0 > UINT64_MAX - a1)
            len0[depth] = UINT64_MAX;
        else
            len0[depth] = a0 + a1;

        /* len1[depth] = 3*len0[depth-1] + len1[depth-1] */
        if (a0 > (UINT64_MAX - a1) / 3)
            len1[depth] = UINT64_MAX;
        else
            len1[depth] = a0 * 3 + a1;
    }

    return get_symbol(n, depth, 0, len0, len1);
}
