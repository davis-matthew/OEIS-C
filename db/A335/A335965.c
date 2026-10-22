/*
 * A335965  a(n) = number of odd numbers in the n-th row of the Narayana
 *          triangle A001263.
 * https://oeis.org/A335965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of odd Narayana numbers in row n of the Narayana
 *            triangle
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (OEIS A335965) The implementation uses Kummer's theorem to test parity of each
   Narayana number without computing it. */

#include <stdint.h>
#include <stdlib.h>

/* Count trailing zeros of a non‑zero 64‑bit integer. */
static unsigned int ctz_uint64(uint64_t v)
{
    unsigned int t = 0;
    while ((v & 1ULL) == 0) {
        v >>= 1;
        ++t;
    }
    return t;
}

seq_t a(idx_t n)
{
    /* static cache of computed results */
    static uint64_t *odd_cache = NULL;      /* odd entry counts per n (index 0 unused) */
    static unsigned int *v2_fact = NULL;    /* v2(i!) for 0 <= i <= max_n */
    static idx_t max_n = 0;                 /* current cache size */

    if (n == 0) return (seq_t)0;   /* for safety – offset starts at 1 */

    /* extend the cache to include n */
    if (n > max_n) {
        idx_t new_max = n;

        /* realloc with explicit casts for C++ compatibility */
        odd_cache = (uint64_t *)realloc(odd_cache, sizeof(uint64_t) * (new_max + 1));
        v2_fact   = (unsigned int *)realloc(v2_fact,   sizeof(unsigned int) * (new_max + 1));

        /* initialise v2_fact[0] and odd_cache[0] for the new space */
        v2_fact[0]   = 0;
        odd_cache[0] = 0;

        /* compute v2 factorials up to new_max */
        for (idx_t i = max_n + 1; i <= new_max; ++i) {
            unsigned int tz = ctz_uint64((uint64_t)i);
            v2_fact[i] = v2_fact[i - 1] + tz;
        }

        /* compute odd counts for rows that were not yet computed */
        for (idx_t nn = max_n + 1; nn <= new_max; ++nn) {
            uint64_t cnt = 0;
            unsigned int v2_nm1 = v2_fact[nn - 1];
            unsigned int v2_n   = v2_fact[nn];

            for (idx_t k = 1; k <= nn; ++k) {
                unsigned int v2A = v2_nm1 - v2_fact[k - 1] - v2_fact[nn - k];
                unsigned int v2B = v2_n   - v2_fact[k - 1] - v2_fact[nn - k + 1];
                unsigned int v2k = ctz_uint64((uint64_t)k);
                if (v2A + v2B == v2k)
                    ++cnt;
            }
            odd_cache[nn] = cnt;
        }
        max_n = new_max;
    }

    return (seq_t)odd_cache[n];
}
