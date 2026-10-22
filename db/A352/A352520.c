/*
 * A352520  Number of integer compositions y of n with exactly one nonfixed
 *          point y(i) != i.
 * https://oeis.org/A352520
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
 * Computes a(n) – the number of integer compositions y of n with exactly
 * one non‑fixed point y(i) ≠ i.
 * For each n ≥ 0 it iterates over all possible lengths k of a composition
 * and all positions j where the part may differ from its index.
 * A composition is valid when the computed part t = n – (k(k+1)/2) + j
 * is positive and t ≠ j.  All other parts are equal to their indices.
 * The function therefore counts all pairs (k, j) satisfying these
 * conditions.  This runs in O(√n) for each n.  
 */

seq_t a(idx_t n)
{
    seq_t count = 0;

    for (idx_t k = 1; ; ++k)
    {
        /* S = k(k+1)/2, the sum of the fixed parts 1..k */
        seq_t S = (seq_t)k * (k + 1) / 2;

        /* If even the smallest possible part at position j = k
         * would make the sum too large, no larger k can work. */
        seq_t Sk = S - (seq_t)k;
        if (Sk >= (seq_t)n)
            break;

        for (idx_t j = 1; j <= k; ++j)
        {
            seq_t sum = (seq_t)n + (seq_t)j;
            if (sum <= S)   /* would give a non‑positive part */
                continue;

            seq_t t = sum - S;   /* the part at position j */
            if (t == (seq_t)j)   /* would be a fixed point */
                continue;

            ++count;
        }
    }

    return count;
}
