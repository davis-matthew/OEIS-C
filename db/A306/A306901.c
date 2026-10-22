/*
 * A306901  Sum over all partitions of n of the bitwise AND of the parts.
 * https://oeis.org/A306901
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{b≥0} 2^b · p_b(n) where p_b(n) = number of partitions
 *            of n using only parts whose b‑th bit is 1
 * Range:     u8 n <= 21, u16 n <= 69, u32 n <= 225, u64 n <= 773,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*=====================================================================*/
#include <stdlib.h>

/*--------------------------------------------------------------
 *  a(n) – compute the nth term of OEIS A306901
 *  n  – index (idx_t is an unsigned integer type)
 *  return – seq_t type value
 *--------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* static cache: partitions with each bit constrained */
    static idx_t  max_n   = 0;      /* maximum n processed      */
    static seq_t **p_cache = NULL;   /* p_cache[b][i] = p_b(i)  */
    static idx_t  max_bit = 0;       /* highest bit cached      */

    /* a(0) is defined to be 0 */
    if (n == 0)
        return (seq_t)0;

    /* enlarge cache if a larger n is requested */
    if (n > max_n)
    {
        /* free old cache */
        if (p_cache != NULL) {
            for (idx_t i = 0; i <= max_bit; ++i)
                free(p_cache[i]);
            free(p_cache);
        }

        /* set new limits */
        max_n = n;
        idx_t tmp = max_n;
        max_bit  = 0;
        while (tmp > 1) {
            tmp >>= 1;
            ++max_bit;
        }

        /* allocate cache for 0 … max_bit */
        p_cache = (seq_t**)malloc((max_bit + 1) * sizeof(seq_t*));
        for (idx_t b = 0; b <= max_bit; ++b) {
            /* each row has indices 0 … max_n */
            p_cache[b] = (seq_t*)calloc((size_t)(max_n + 1), sizeof(seq_t));
            if (p_cache[b] == NULL) /* large n may exhaust memory */
                return (seq_t)0;     /* silently return 0 on failure   */

            /* in situ DP for partitions using only parts with bit b set */
            seq_t *dp = p_cache[b];
            dp[0] = (seq_t)1;          /* only one way to sum to 0 */
            for (idx_t k = 1; k <= max_n; ++k)
            {
                if (((k >> b) & 1ULL) != 0)      /* part k has bit b = 1 */
                {
                    for (idx_t i = k; i <= max_n; ++i)
                        dp[i] += dp[i - k];
                }
            }
        }
    }

    /* compute a(n) from cached values */
    seq_t result = (seq_t)0;
    for (idx_t b = 0; ( (1ULL << b) <= n ); ++b)
    {
        seq_t powb = (seq_t)(1ULL << b);
        result += powb * p_cache[b][n];
    }
    return result;
}
