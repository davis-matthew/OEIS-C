/*
 * A358068  Numbers that share a (decimal) digit with the sum of their proper
 *          divisors.
 * https://oeis.org/A358068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A358068: numbers that share a decimal digit with the sum
   of their proper divisors. The function builds the sequence on demand
   by scanning all integers, caching results for fast subsequent
   accesses. */

#include <stdlib.h>
#include <stdbool.h>

/* Return a bitmask of decimal digits present in x.  Bit i corresponds
   to digit i. */
static inline int digit_mask(seq_t x) {
    int mask = 0;
    if (x == (seq_t)0) return 1;          /* digit 0 present */
    while (x != (seq_t)0) {
        int d = (int)(x % (seq_t)10);
        mask |= 1 << d;
        x /= (seq_t)10;
    }
    return mask;
}

/* Integer square root of a 64‑bit unsigned integer. */
static inline uint64_t isqrt_u64(uint64_t n) {
    uint64_t x = n;
    uint64_t y = (x + 1) >> 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

/* Ensure the cache array has capacity for at least 'needed' elements. */
static inline void ensure_capacity(size_t needed, size_t *cap, void **ptr) {
    if (needed > *cap) {
        size_t new_cap = (*cap == 0) ? 8 : (*cap << 1);
        if (new_cap < needed) new_cap = needed;
        *ptr = realloc(*ptr, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

seq_t a(idx_t n) {
    /* Static state for memoisation */
    static seq_t *cache = NULL;         /* array of found terms */
    static size_t cache_len = 0;        /* number of terms stored */
    static size_t cache_cap = 0;        /* capacity of the array */
    static uint64_t curr = 1;           /* next number to examine */

    while (cache_len < n) {
        uint64_t num = curr++;          /* candidate integer */

        /* Compute sum of proper divisors of num */
        seq_t sum = 0;
        if (num > 1) {
            sum = 1;                   /* divisor 1 */
            uint64_t r = isqrt_u64(num);
            for (uint64_t i = 2; i <= r; ++i) {
                if (num % i == 0) {
                    sum += (seq_t)i;
                    uint64_t j = num / i;
                    if (j != i)          /* avoid double‑counting sqrt */
                        sum += (seq_t)j;
                }
            }
        }

        /* Check for shared decimal digit */
        if (digit_mask((seq_t)num) & digit_mask(sum)) {
            /* Store in cache */
            ensure_capacity(cache_len + 1, &cache_cap, (void **)&cache);
            cache[cache_len++] = (seq_t)num;
        }
    }

    return cache[n - 1];
}
