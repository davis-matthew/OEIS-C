/*
 * A337853  a(n) is the number of partitions of n as the sum of two Niven
 *          numbers.
 * https://oeis.org/A337853
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1073, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of unordered partitions of n into two Niven numbers */

#include <stdlib.h>
#include "../utils/digit_sum.h"

/* static cache of Niven status up to the largest n requested so far */
static uint8_t *niven_flag = NULL;   /* 1 if Niven, 0 otherwise */
static idx_t cached_max = 0;

/* Build Niven flag array up to 'up_to' */
static void ensure_niven_cache(idx_t up_to) {
    if (up_to <= cached_max) return;  /* nothing to do */

    /* allocate new array */
    uint8_t *new_flag = (uint8_t *)realloc(niven_flag, (size_t)(up_to + 1));
    if (new_flag == NULL) {
        /* allocation failed – cannot continue */
        exit(EXIT_FAILURE);
    }
    niven_flag = new_flag;

    /* compute new entries */
    for (idx_t i = cached_max + 1; i <= up_to; ++i) {
        seq_t sum = digit_sum((seq_t)i);          /* digit sum of i */
        niven_flag[i] = (sum != (seq_t)0 && ((seq_t)i % sum == (seq_t)0)) ? 1U : 0U;
    }
    cached_max = up_to;
}

seq_t a(idx_t n) {
    /* compute and cache Niven numbers up to n */
    ensure_niven_cache(n);

    seq_t cnt = 0;
    for (idx_t i = 1; i <= n / 2; ++i) {
        if (niven_flag[i] && niven_flag[n - i]) {
            ++cnt;
        }
    }
    return cnt;
}
