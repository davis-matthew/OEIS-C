/*
 * A362960  Length of longest Roman numeral representation of a number <= n.
 * https://oeis.org/A362960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = max_{i=1..n} A006968(i) (length of Roman numeral
 *            representation)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t roman_len(idx_t m)
{
    /* Precomputed lengths for Roman numerals for each digit position */
    static const unsigned char len_ones[10]   = {0,1,2,3,2,1,2,3,4,2};
    static const unsigned char len_tens[10]   = {0,1,2,3,2,1,2,3,4,2};
    static const unsigned char len_hund[10]   = {0,1,2,3,2,1,2,3,4,2};
    static const unsigned char len_thou[11]   = {0,1,2,3,4,5,6,7,8,9,10};

    idx_t thousands = m / 1000u;
    idx_t rem      = m % 1000u;
    idx_t hundreds = rem / 100u;
    idx_t tens      = (rem / 10u) % 10u;
    idx_t ones      = rem % 10u;

    seq_t total = (seq_t)len_thou[thousands]
                + (seq_t)len_hund[hundreds]
                + (seq_t)len_tens[tens]
                + (seq_t)len_ones[ones];
    return total;
}

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)0;

    static seq_t *cache = NULL;
    static idx_t cache_size = 0u;

    /* Grow cache to accommodate up to n */
    if (n > cache_size)
    {
        size_t new_size = (size_t)n;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache)
            return (seq_t)0;  /* Allocation failure: return 0 */
        cache = new_cache;

        /* Compute and store a(i) for all new indices */
        seq_t current_max = (cache_size > 0u) ? cache[cache_size - 1] : (seq_t)0;
        for (idx_t i = cache_size + 1u; i <= n; ++i)
        {
            seq_t len = roman_len(i);
            if (len > current_max)
                current_max = len;
            cache[i - 1u] = current_max;
        }
        cache_size = n;
    }

    return cache[n - 1u];
}
