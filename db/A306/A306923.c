/*
 * A306923  Sum over all partitions of n into distinct parts of the bitwise
 *          AND of the parts.
 * https://oeis.org/A306923
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{partitions of n into distinct parts} (bitwise AND of
 *            the parts)
 * Range:     u8 n <= 33, u16 n <= 119, u32 n <= 411, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) is computed by counting, for each bit k, the number of partitions
 *  whose parts all contain that bit, then summing 2^k times that count.    */

#include <stdlib.h>

static idx_t  computed_max   = 0;     /* largest n for which a(seq) is known           */
static seq_t *a_cache        = NULL;  /* computed values of a(n) up to computed_max  */
static seq_t **g_cache      = NULL;   /* g_cache[k][s] = #partitions of s using only
                                        parts that have bit k set                   */
static int    cached_bits   = 0;      /* number of bit tables currently allocated     */

seq_t a(idx_t n)
{
    /* Handle n==0 specially (empty partition) */
    if (n == 0) {
        if (!a_cache) {
            a_cache = (seq_t *)malloc(sizeof(seq_t));
            a_cache[0] = 0;
            computed_max = 0;
        }
        return a_cache[0];
    }

    /* Re‑compute the sequence if a higher n is requested */
    if (a_cache == NULL || n > computed_max)
    {
        /* Allocate array for a(n) */
        seq_t *new_a = (seq_t *)realloc(a_cache, (n + 1) * sizeof(seq_t));
        if (!new_a) return 0;          /* allocation failed – give up */
        a_cache = new_a;

        /* Determine number of bits needed for the largest part (n) */
        int bits = 0;
        while ((idx_t)1 << bits <= n) ++bits;  /* bits is now ceil(log2(n+1)) */
        int bits_count = bits;                  /* bits_count = floor(log2(n)) + 1 */

        /* Free old g_cache tables, if any */
        if (g_cache) {
            for (int i = 0; i < cached_bits; ++i) free(g_cache[i]);
            free(g_cache);
        }

        /* Allocate new tables for each bit */
        g_cache = (seq_t **)malloc(bits_count * sizeof(seq_t *));
        for (int b = 0; b < bits_count; ++b)
            g_cache[b] = (seq_t *)calloc(n + 1, sizeof(seq_t));

        cached_bits = bits_count;

        /* Base case: zero sum can always be formed by the empty set */
        for (int b = 0; b < bits_count; ++b) g_cache[b][0] = 1;

        /* 0‑1 knapsack DP for each bit table */
        for (idx_t part = 1; part <= n; ++part)
        {
            unsigned int mask = (unsigned int)part;   /* part is small enough */
            for (int b = 0; b < bits_count; ++b)
            {
                if (mask & (1u << b))
                {
                    seq_t *dp = g_cache[b];
                    /* Update dp from high to low to avoid reuse of this part */
                    for (size_t s = n; s >= (size_t)part; --s)
                    {
                        dp[s] += dp[s - part];
                        if (s == (size_t)part) break; /* prevent wrap‑around */
                    }
                }
            }
        }

        /* Compute the sequence values from the g_cache tables */
        for (idx_t s = 1; s <= n; ++s)
        {
            seq_t val = 0;
            for (int b = 0; b < bits_count; ++b)
            {
                seq_t cnt = g_cache[b][s];
                if (cnt)
                    val += ((seq_t)1 << b) * cnt;
            }
            a_cache[s] = val;
        }

        /* The zero entry stays zero (empty partition) */
        a_cache[0] = 0;
        computed_max = n;
    }

    return a_cache[n];
}
