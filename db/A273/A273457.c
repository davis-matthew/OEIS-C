/*
 * A273457  Even numbers 2n that do not have a Goldbach partition 2n = p + q
 *          (p < q; p, q prime) satisfying sqrt(n) < p <= sqrt(2n).
 * https://oeis.org/A273457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 4883, u32 n <= 9801, u64 n <= 9801,
 *            u128 n <= 9801, big any n
 * Helpers:   alloc_seq_array, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A273457: Even numbers 2n for which there is no Goldbach partition 
   2n = p + q (p < q, both prime) with sqrt(n) < p ≤ sqrt(2n) */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime.h"          /* bool is_prime(idx_t)   */
#include "../utils/alloc_seq_array.h"   /* seq_t *alloc_seq_array(size_t) */

static inline idx_t isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = x;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if ((unsigned __int128)mid * mid <= x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

static void buf_append(seq_t val, seq_t **buf, size_t *size, size_t *cap)
{
    if (*cap == 0) {
        *cap = 16;
        *buf = alloc_seq_array(*cap);
    } else if (*size == *cap) {
        size_t new_cap = *cap << 1;
        seq_t *new_buf = alloc_seq_array(new_cap);
        for (size_t i = 0; i < *size; ++i)
            new_buf[i] = (*buf)[i];
        free(*buf);
        *buf = new_buf;
        *cap = new_cap;
    }
    (*buf)[*size] = val;
    ++(*size);
}

seq_t a(idx_t n)
{
    /* Cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;
    static idx_t next_m = 1;          /* m = n in definition 2n */

    /* Generate further terms until we have at least n of them */
    while (cache_size < n) {
        idx_t m = next_m++;
        idx_t candidate = m << 1;          /* 2m, the even number */

        idx_t p_min = isqrt_u64(m) + 1;
        idx_t p_max = isqrt_u64(candidate);

        bool bad = false;
        for (idx_t p = p_min; !bad && p <= p_max; ++p) {
            if (!is_prime(p))
                continue;
            idx_t q = candidate - p;
            if (p < q && is_prime(q))
                bad = true;                /* Goldbach partition found */
        }

        if (!bad) {
            buf_append((seq_t)candidate, &cache, &cache_size, &cache_cap);
        }
    }

    return cache[n - 1];
}
