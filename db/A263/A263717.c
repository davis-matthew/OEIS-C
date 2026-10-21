/*
 * A263717  Number of partitions of n into perfect odd powers (1 being
 *          excluded).
 * https://oeis.org/A263717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into odd perfect powers (1
 *            excluded)
 * Range:     u8 n <= 402, u16 n <= 999, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Comparator for qsort on uint64_t items */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    return (va < vb) ? -1 : (va > vb);
}

/* Count partitions of n into odd perfect powers (exponent ≥ 2, base ≥ 2) */
seq_t a(idx_t n)
{
    /* the smallest odd perfect power is 3² = 9 */
    if (n < 9)
        return 0;

    /* ---- 1. Build the list of odd perfect powers ≤ n ---- */
    size_t cap = 32;
    size_t cnt = 0;
    uint64_t *powers = (uint64_t *)malloc(cap * sizeof(uint64_t));
    if (!powers)
        return 0;               /* allocation failure */

    for (idx_t base = 2; base <= n / base; ++base)
    {
        idx_t val = base * base;          /* exponent = 2 */
        while (val <= n)
        {
            if (val & 1)                 /* odd values only */
            {
                if (cnt >= cap)
                {
                    cap <<= 1;
                    uint64_t *tmp = (uint64_t *)realloc(powers, cap * sizeof(uint64_t));
                    if (!tmp)
                    {
                        free(powers);
                        return 0;
                    }
                    powers = tmp;
                }
                powers[cnt++] = (uint64_t)val;
            }
            if (val > n / base)          /* prevent overflow for next power */
                break;
            val *= base;                 /* next exponent */
        }
    }

    /* Sort and keep unique values */
    qsort(powers, cnt, sizeof(uint64_t), cmp_uint64);
    size_t uniq = 0;
    for (size_t i = 0; i < cnt; ++i)
    {
        if (uniq == 0 || powers[i] != powers[uniq - 1])
            powers[uniq++] = powers[i];
    }
    cnt = uniq;

    /* ---- 2. Unbounded‑knapsack DP to count partitions ---- */
    size_t size = (size_t)n + 1;
    seq_t *dp = (seq_t *)calloc(size, sizeof(seq_t));
    if (!dp)
    {
        free(powers);
        return 0;                        /* allocation failure */
    }
    dp[0] = 1;

    for (size_t pi = 0; pi < cnt; ++pi)
    {
        idx_t part = (idx_t)powers[pi];
        for (idx_t i = part; i <= n; ++i)
            dp[i] += dp[i - part];
    }

    seq_t result = dp[n];
    free(dp);
    free(powers);
    return result;
}
