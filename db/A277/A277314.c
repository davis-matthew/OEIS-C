/*
 * A277314  Number of nonzero coefficients in Stern polynomial B(n,t).
 * https://oeis.org/A277314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of nonzero coefficients in Stern polynomial B(n,t)
 *            computed by recursively building the set of exponents as a
 *            bitmask. Recurrence: m(0) = 0 m(1) = 1 for n even: m(n) = m(n/2)
 *            << 1 for n odd : m(n) = m((n-1)/2) | m((n+1)/2) a(n) =
 *            popcount(m(n))
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>

static uint32_t *mask_cache = NULL;
static size_t mask_cap   = 0;

/* Ensure the cache can hold the requested index.  Uncomputed
   entries are initialised to UINT32_MAX. */
static void ensure_cache(idx_t n)
{
    if ((size_t)n < mask_cap)
        return;
    size_t new_cap = (size_t)n + 1;
    uint32_t *tmp = (uint32_t *)realloc(mask_cache, new_cap * sizeof(uint32_t));
    if (!tmp) {
        /* Allocation failed – extremely unlikely for test ranges. */
        exit(1);
    }
    /* Initialise new entries as uncomputed (UINT32_MAX). */
    for (size_t i = mask_cap; i < new_cap; ++i)
        tmp[i] = UINT32_MAX;
    mask_cache = tmp;
    mask_cap   = new_cap;
}

/* Recursive helper that returns the bitmask of exponents for B(n,t). */
static uint32_t get_mask(idx_t n)
{
    ensure_cache(n);
    uint32_t res = mask_cache[n];
    if (res != UINT32_MAX)      /* already computed */
        return res;

    if (n == 0) {
        res = 0U;
    } else if (n == 1) {
        res = 1U;               /* exponent 0 is set */
    } else if ((n & 1ULL) == 0ULL) {   /* n even */
        res = get_mask(n >> 1) << 1;
    } else {                       /* n odd */
        idx_t m = n >> 1;           /* (n-1)/2  , (n+1)/2 = m and m+1 */
        res = get_mask(m) | get_mask(m + 1);
    }
    mask_cache[n] = res;
    return res;
}

seq_t a(idx_t n)
{
    uint32_t mask = get_mask(n);
    /* Count set bits in a 32‑bit integer.  __builtin_popcount is available
       on all supported compilers (GCC/Clang/MSVC). */
    int pc = __builtin_popcount(mask);
    return (seq_t)pc;
}
