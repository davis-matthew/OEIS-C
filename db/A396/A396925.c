/*
 * A396925  Numbers k which have a divisor d such that d^2 < k and phi(d) =
 *          phi(k/d), where phi = A000010.
 * https://oeis.org/A396925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   numbers k having divisor d such that d^2 < k and φ(d)=φ(k/d)
 *            (OEIS A396925). The function implements a linear search with
 *            caching; it iterates over integers and checks the divisor
 *            condition
 * Range:     u8 n <= 17, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   calc_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/calc_phi.h"

/* Helper: returns true if k has a divisor d with d^2 < k and φ(d)=φ(k/d). */
static bool has_condition(seq_t k)
{
    uint64_t kv = (uint64_t)k;          /* assume k fits in 64 bits for n up to 10000 */
    for (uint64_t d = 1; d * d < kv; ++d) {
        if ((k % (seq_t)d) == 0) {      /* d divides k */
            seq_t d_seq = (seq_t)d;
            seq_t e_seq = k / d_seq;
            if (calc_phi(d_seq) == calc_phi(e_seq))
                return true;
        }
    }
    return false;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;      /* number of terms stored */
    static size_t cache_cap = 0;      /* allocated capacity */
    static uint64_t last_k = 1;       /* last tested integer */

    while (cache_len < (size_t)n) {
        ++last_k;
        if (has_condition((seq_t)last_k)) {
            /* grow cache if necessary */
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = 2 * (cache_cap + 1);
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)last_k;
        }
    }
    return cache[(size_t)n - 1];
}
