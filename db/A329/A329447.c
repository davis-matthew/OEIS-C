/*
 * A329447  Start with a(0)=0; thereafter, look left and identify the least
 *          frequent digit d so far (in case of a tie, choose the smallest d):
 *          after then a(n) = 10c + d, where c > 0 is the number of times d
 *          has appeared so far.
 * https://oeis.org/A329447
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 135, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 0.
   For n > 0, let d be the least frequently occurring digit among
   the digits of all previous terms (ignore digits that have not yet
   appeared, breaking ties by the smallest digit).  Let c be the
   count of that digit so far.  Then a(n) = 10 c + d.  */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/get_digits.h"

seq_t a(idx_t n)
{
    /* Static cache of computed terms and digit frequencies.  */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t max_calc = 0;          /* largest n already computed */
    static unsigned long long freq[10] = {0};

    /* Initialise on first call.  */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)0;
        max_calc = 0;
        freq[0] = 1;                    /* a(0) contributes one digit '0' */
    }

    /* If requested term already cached, return it.  */
    if (n <= max_calc)
        return cache[n];

    /* Grow the cache if necessary.  */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;   /* double until enough */
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute missing terms one by one.  */
    for (idx_t i = max_calc + 1; i <= n; ++i) {
        /* Find the least frequent digit that has appeared.  */
        unsigned long long best_cnt = (unsigned long long)-1;   /* UINT64_MAX */
        unsigned best_d   = 0;
        for (int d = 0; d < 10; ++d) {
            unsigned long long f = freq[d];
            if (f == 0) continue;          /* ignore digits that have not appeared */
            if (f < best_cnt || (f == best_cnt && d < best_d)) {
                best_cnt = f;
                best_d   = (unsigned)d;
            }
        }

        unsigned long long c = best_cnt;
        unsigned d           = best_d;

        /* Compute the new term.  */
        seq_t val = (seq_t)((seq_t)10 * (seq_t)c + (seq_t)d);
        cache[i]  = val;

        /* Update the global digit counts with the digits of val.  */
        size_t len;
        uint8_t buf[128];                  /* sufficient for all values tested */
        get_digits(val, buf, &len);
        for (size_t k = 0; k < len; ++k)
            ++freq[buf[k]];
    }

    max_calc = n;
    return cache[n];
}
