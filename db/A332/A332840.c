/*
 * A332840  Number of fixed-point free involutions in a fixed Sylow 2-subgroup
 *          of the symmetric group of degree 2n.
 * https://oeis.org/A332840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 29, u64 n <= 59, u128 n <= 117,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Π{ G[i] | i is a set bit in the binary expansion of n }
 * where G[0] = 1 and for i ≥ 1
 *        G[i] = G[i‑1]^2 + 2^(2^i – 1)
 * (This follows from the product definition of A332840 and the
 * recurrence for the auxiliary sequence A332758.)
 */

#include <stdbool.h>
#include "../utils/seq_pow.h"

static void ensure_G_upto(idx_t upto, seq_t *G, bool *computed)
{
    for (idx_t i = 1; i <= upto; ++i)
    {
        if (!computed[i])
        {
            /* exp = 2^i – 1 */
            idx_t exp = (((idx_t)1) << i) - 1;
            seq_t two = seq_pow((seq_t)2, exp);
            G[i] = G[i - 1] * G[i - 1] + two;
            computed[i] = true;
        }
    }
}

seq_t a(idx_t n)
{
    /* static tables for the G[i] values */
    static seq_t G[65] = {0};
    static bool computed[65] = {0};
    static bool initialized = false;

    if (!initialized)
    {
        initialized = true;
        G[0] = (seq_t)1;
        computed[0] = true;
    }

    seq_t result = (seq_t)1;
    idx_t bit = 0;
    idx_t m = n;

    while (m)
    {
        if (m & 1ULL)
        {
            if (!computed[bit])
                ensure_G_upto(bit, G, computed);
            result *= G[bit];
        }
        m >>= 1;
        ++bit;
    }
    return result;
}
