/*
 * A324298  Positive integers k such that 10*k+6 is equal to the product of
 *          two integers ending with 6 (A324297).
 * https://oeis.org/A324298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th integer k such that 10*k+6 is a product of two
 *            integers ending with 6. Equivalently, k = 10*a*b + 6*(a+b)+3 for
 *            some non‑negative integers a,b. The algorithm here tests each
 *            odd integer k for existence of a divisor f of (10*k+6) with f≡6
 *            (mod 10) and (10*k+6)/f≡6 (mod 10). Result...
 * Range:     u8 n <= 59, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

/* Integer square root for uint64_t; only the part used in k_ok. */
static inline uint64_t isqrt_u64(uint64_t x)
{
    uint64_t r = 0, bit;
    /* bit starts at the largest power of four <= x */
    bit = 1ULL << 62;           /* 2^62  (>= 2^63 for unsigned long long) */
    while (bit > x) bit >>= 2;
    while (bit != 0) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

/* Test whether a given integer k satisfies the product condition. */
static inline bool ok_k(seq_t k)
{
    uint64_t n64 = ((uint64_t)k) * 10ULL + 6ULL;   /* N = 10*k+6 */
    uint64_t sqrt_n = isqrt_u64(n64);
    for (uint64_t d = 6; d <= sqrt_n; ++d) {
        if (d % 10 != 6) continue;                /* divisor must end with 6 */
        if (n64 % d != 0) continue;
        uint64_t g = n64 / d;
        if (g % 10 != 6) continue;
        return true;
    }
    return false;
}

/* Cached array of previously found terms. */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;     /* number of terms stored */
static size_t cache_cap  = 0;     /* allocated capacity */
static idx_t  last_tested = 0;    /* last integer examined */

/* Ensure there is room for at least one more term. */
static inline void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr) exit(EXIT_FAILURE);  /* abort on allocation failure */
    cache      = new_ptr;
    cache_cap  = new_cap;
}

/* Main function required by the interface. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* out of range, but harmless */

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        ++last_tested;
        if ((last_tested & 1) == 0) continue;   /* all terms are odd */

        if (ok_k((seq_t)last_tested)) {
            if (cache_len >= cache_cap) grow_cache();
            cache[cache_len++] = (seq_t)last_tested;
        }
    }
    return cache[(size_t)n - 1];
}
