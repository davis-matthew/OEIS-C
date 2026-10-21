/*
 * A262466  Number of (n+1) X (2+1) 0..1 arrays with each row divisible by 3
 *          and each column divisible by 5, read as a binary number with top
 *          and left being the most significant bits.
 * https://oeis.org/A262466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 4*a(n-2) + 4*a(n-3) + 8*a(n-4) - 44*a(n-5) +
 *            44*a(n-6) - 44*a(n-7) + 33*a(n-8); Initial values: a(1)=1,
 *            a(2)=3, a(3)=9, a(4)=17, a(5)=37, a(6)=107, a(7)=321, a(8)=865.
 *            Uses memoization
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 42, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static const seq_t init_vals[8] = {1, 3, 9, 17, 37, 107, 321, 865};
static seq_t *cache = NULL;   /* cache[0] corresponds to a(1) */
static idx_t cache_size = 0;  /* number of terms currently cached */

/* Compute a(n) using the linear recurrence with memoization. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for n<1, return 0 */

    /* Return precomputed initial values directly. */
    if (n <= 8) return init_vals[n - 1];

    /* Allocate and initialize cache if necessary. */
    if (cache_size == 0) {
        cache = (seq_t*)malloc(sizeof(seq_t) * 8);
        memcpy(cache, init_vals, sizeof(init_vals));
        cache_size = 8;
    }

    /* Extend cache up to n if needed. */
    if (n > cache_size) {
        seq_t *new_cache = (seq_t*)realloc(cache, sizeof(seq_t) * n);
        if (!new_cache) {
            /* Allocation failed – fall back to a new allocation. */
            new_cache = (seq_t*)malloc(sizeof(seq_t) * n);
            if (!new_cache) {
                /* Out of memory: return 0 (caller should not reach here). */
                return (seq_t)0;
            }
            memcpy(new_cache, cache, sizeof(seq_t) * cache_size);
        }
        cache = new_cache;

        /* Compute new terms sequentially. */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            /* Map n -> cache indexes (0‑based).  i-1 is the current index. */
            seq_t a1 = cache[i - 2];
            seq_t a2 = cache[i - 3];
            seq_t a3 = cache[i - 4];
            seq_t a4 = cache[i - 5];
            seq_t a5 = cache[i - 6];
            seq_t a6 = cache[i - 7];
            seq_t a7 = cache[i - 8];
            seq_t a8 = cache[i - 9];  /* i >= 9 here */

            /* Accumulate positive and negative parts separately. */
            seq_t pos = 0;
            seq_t neg = 0;

            /* Positive contributions */
            pos += 4 * a1;
            pos += 4 * a3;
            pos += 8 * a4;
            pos += 44 * a6;
            pos += 33 * a8;

            /* Negative contributions */
            neg += 4 * a2;
            neg += 44 * a5;
            neg += 44 * a7;

            /* Final value: pos - neg (pos >= neg for the sequence). */
            cache[i - 1] = pos - neg;
        }

        cache_size = n;
    }

    return cache[n - 1];
}
