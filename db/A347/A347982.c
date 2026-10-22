/*
 * A347982  a(n) is the greatest k, 0 < k < n, such that tau(k) = tau(n), or
 *          -1 if no such k exists, where tau is A000005.
 * https://oeis.org/A347982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = greatest k < n such that tau(k) = tau(n), or -1 if none
 * Range:     i8 n <= 129, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *tau_cache = NULL;   /* divisor counts[0..size] */
static idx_t tau_cache_size = 0;  /* current maximum index stored */

/* rebuild divisor count cache up to up_to inclusive */
static void ensure_tau_cache(idx_t up_to)
{
    if (up_to < 1)
        return;
    if (tau_cache && tau_cache_size > up_to)
        return;                    /* already enough */

    /* allocate new array */
    if (tau_cache) {
        free(tau_cache);
    }
    tau_cache = (seq_t *)malloc((up_to + 1) * sizeof(seq_t));
    if (!tau_cache) abort();        /* memory fail */

    for (idx_t i = 0; i <= up_to; ++i) {
        tau_cache[i] = 0;
    }
    for (idx_t i = 1; i <= up_to; ++i) {
        for (idx_t j = i; j <= up_to; j += i) {
            tau_cache[j]++;         /* count divisor i of j */
        }
    }
    tau_cache_size = up_to;
}

seq_t a(idx_t n)
{
    if (n == 0)               /* offset starts at 1, ignore 0 */
        return (seq_t)(-1);

    ensure_tau_cache(n);       /* need tau(n) */
    seq_t tau_n = tau_cache[n];

    for (idx_t k = n - 1; k > 0; k--) {
        if (tau_cache[k] == tau_n)
            return (seq_t)k;
    }
    return (seq_t)(-1);
}
