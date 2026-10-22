/*
 * A379308  Number of integer partitions of n with a unique squarefree part.
 * https://oeis.org/A379308
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{s squarefree <= n} p_sf(n - s) where p_sf(k) is the
 *            number of partitions of k into squareful parts (parts that are
 *            not squarefree)
 * Range:     u8 n <= 42, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* static memoisation */
static idx_t max_n = 0;
static seq_t *dp_sf = NULL;       /* dp_sf[i] = partitions of i into squareful parts */
static size_t dp_len = 0;
static uint8_t *is_sqfree = NULL; /* 1 if i is squarefree, 0 otherwise */
static size_t sqfree_len = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* no parts can be squarefree */

    /* Extend memoised data to cover n */
    if (n > max_n) {
        /* Allocate larger arrays if necessary */
        size_t new_len = (size_t)(n + 1);

        dp_sf = (seq_t*)realloc(dp_sf, new_len * sizeof(seq_t));
        if (!dp_sf) abort();
        dp_len = new_len;

        is_sqfree = (uint8_t*)realloc(is_sqfree, new_len * sizeof(uint8_t));
        if (!is_sqfree) abort();
        sqfree_len = new_len;

        /* Compute squarefree flags up to n */
        for (idx_t i = 0; i <= n; ++i)
            is_sqfree[i] = 1;                /* assume squarefree */
        /* Eliminate multiples of squares */
        for (idx_t p = 2; (idx_t)p * p <= n; ++p) {
            idx_t step = (idx_t)p * p;
            for (idx_t m = step; m <= n; m += step)
                is_sqfree[m] = 0;
        }

        /* Compute dp_sf: partitions into squareful parts */
        dp_sf[0] = 1;                 /* empty partition */
        for (idx_t i = 1; i <= n; ++i)
            dp_sf[i] = 0;

        /* For each squareful coin (i.e., not squarefree and >1) */
        for (idx_t coin = 4; coin <= n; ++coin) {   /* coin starts at 4 (i.e., 2^2) */
            if (is_sqfree[coin]) continue;          /* skip squarefree parts */
            for (idx_t sum = coin; sum <= n; ++sum)
                dp_sf[sum] += dp_sf[sum - coin];
        }

        max_n = n;
    }

    /* Count partitions with a unique squarefree part */
    seq_t result = 0;
    for (idx_t s = 1; s <= n; ++s) {
        if (is_sqfree[s]) {
            result += dp_sf[n - s];
        }
    }
    return result;
}
