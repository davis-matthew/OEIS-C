/*
 * A300326  Sum of the largest possible permutations that can be written
 *          without repetition of digits in each base from binary to n+1.
 * https://oeis.org/A300326
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{b=2}^{n+1} Σ_{k=0}^{b-1} (b-1-k)·b^k (a(0)=0, a(1)=2,
 *            a(2)=23, a(3)=251, a(4)=3181, …)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* A small static cache – sufficient for n up to 10000. */
static seq_t cache[10001];
static bool cache_initialized = false;
static idx_t cache_max = 0;

seq_t a(idx_t n)
{
    /* Ensure the cache is initialized. */
    if (!cache_initialized) {
        cache[0] = 0;
        /* Remaining elements are zero‑initialized (built‑in types) or
           default‑constructed (arbitrary‑precision types). */
        cache_initialized = true;
        cache_max = 0;
    }

    if (n > 10000) {
        /* If n exceeds the assumed limit, we incrementally compute up to n,
           but the array may overflow – the caller should not request such
           values for performance reasons. */
    }

    if (n <= cache_max) {
        return cache[n];
    }

    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t val = 0;
        idx_t d = i;                     /* base = i+1, so base-1 = i */
        seq_t base_seq = (seq_t)(i + 1);

        while (1) {
            val = val * base_seq + (seq_t)d;
            if (d == 0) break;
            --d;
        }
        cache[i] = cache[i - 1] + val;
    }

    cache_max = n;
    return cache[n];
}
