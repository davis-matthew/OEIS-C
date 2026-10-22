/*
 * A396005  Numbers k such that tau(2*k) = tau(k + 2) where tau = A000005.
 * https://oeis.org/A396005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 8888, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A396005: numbers k such that τ(2*k) = τ(k + 2) (searched by brute‑force) */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* Undefined for n < 1 */

    /* Cache of previously found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t next_k = 1;           /* next k to test */

    /* Generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 8;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();           /* Allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        uint64_t k = next_k;
        uint64_t d1 = divisor_count_u64(2 * k);   /* τ(2*k) */
        uint64_t d2 = divisor_count_u64(k + 2);   /* τ(k+2) */
        if (d1 == d2) {
            cache[cache_len++] = (seq_t)k;       /* store found k */
        }
        ++next_k;
    }

    return cache[n - 1];
}
