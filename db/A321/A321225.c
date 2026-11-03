/*
 * A321225  Break the infinite word 1232112321... into chunks so that the sum
 *          of the digits in the n-th chunk is n.
 * https://oeis.org/A321225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 100001*a(n-9) - 100000*a(n-18) for n>18 Initial values:
 *            a(1)=1, a(2)=2, a(3)=3, a(4)=211, a(5)=23, a(6)=2112, a(7)=3211,
 *            a(8)=2321, a(9)=12321, a(10)=123211, a(11)=232112, a(12)=321123,
 *            a(13)=21123211,a(14)=2321123, a(15)=211232112,
 *            a(16)=321123211,a(17)=232112321,a(18)=1232112321
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 71,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 1775 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* helper that constructs the nth chunk of the infinite word 12321… */
static seq_t compute_chunk(idx_t tgt)
{
    static const unsigned pattern[5] = {1, 2, 3, 2, 1};
    unsigned pos = 0;          /* position in pattern cycle */
    unsigned sum = 0;          /* current sum of digits in the chunk */
    unsigned cur  = 1;         /* current chunk number */
    seq_t    val  = 0;         /* value of the current chunk being built */

    while (1) {
        unsigned d = pattern[pos];
        pos++;
        if (pos == 5) pos = 0;

        sum += d;
        val  = val * 10 + d;

        if (sum == cur) {          /* finished a chunk */
            if (cur == tgt) return val;   /* desired chunk found */
            cur++;                    /* advance to next chunk */
            sum = 0;
            val = 0;
        }
    }
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* array a[1..*] (index 0 unused) */
    static size_t cache_len = 0;  /* largest index already computed */

    if (n == 0) return 0;         /* safety; not used by OEIS */

    /* extend cache if necessary */
    if (n > cache_len) {
        size_t new_cap = n + 1;    /* +1 to use 1‑based indexing */
        if (cache == NULL) {
            cache = mem_new(seq_t, new_cap);
        } else {
            cache = mem_resize(seq_t, cache, new_cap);
        }
        /* compute missing terms */
        for (size_t i = cache_len + 1; i <= n; ++i) {
            if (i <= 18) {
                cache[i] = compute_chunk((idx_t)i);
            } else {
                cache[i] = cache[i - 9] * (seq_t)100001
                           - cache[i - 18] * (seq_t)100000;
            }
        }
        cache_len = n;
    }
    return cache[n];
}
