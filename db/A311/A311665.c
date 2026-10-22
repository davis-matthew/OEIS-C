/*
 * A311665  Coordination sequence Gal.6.481.6 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311665
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Linear recurrence taken from OEIS A311665 a(n) = a(n-2) - a(n-4)
 *            + a(n-6) + a(n-7) - a(n-8) - a(n-9) + a(n-10) + a(n-11) -
 *            a(n-13) + a(n-15) - a(n-17) for n > 19. Initial values a(0) …
 *            a(23) are supplied. The function memoizes results in a static
 *            array. Works for any seq_t that fits in signed 64‑...
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* 1‑based indices for cache are size_t for array indexing */
    static const seq_t init_vals[24] = {
        1ULL, 4ULL, 8ULL, 13ULL, 14ULL, 20ULL, 28ULL, 30ULL,
        36ULL, 36ULL, 40ULL, 45ULL, 54ULL, 58ULL, 62ULL, 61ULL,
        62ULL, 72ULL, 78ULL, 86ULL, 90ULL, 88ULL, 88ULL, 93ULL
    };

    /* We only need up to about 10 000, so a static array is sufficient */
    #define MAX_CACHE 12000
    static seq_t cache[MAX_CACHE];
    static size_t computed = 0;          /* first index that has been
                                            computed (exclusive) */

    /* Initialise cache once */
    if (computed == 0) {
        for (size_t i = 0; i < sizeof(init_vals)/sizeof(init_vals[0]); ++i)
            cache[i] = init_vals[i];
        computed = 24;   /* indices 0..23 filled */
    }

    if ((idx_t)n < 24)                 /* small indices are already known */
        return cache[(size_t)n];

    /* Compute values up to n if necessary */
    if ((idx_t)computed <= n) {
        /* Ensure array is large enough */
        if ((size_t)n >= MAX_CACHE) {
            /* For very large n, fall back to dynamic allocation using
               malloc/realloc.  This section is not expected for the
               typical problem limits.                      */
            static seq_t *dyn = NULL;
            static size_t dyn_cap = 0;
            static size_t dyn_computed = 0;

            if (!dyn || dyn_cap <= (size_t)n) {
                size_t new_cap = dyn_cap ? dyn_cap*2 : 256;
                while (new_cap <= (size_t)n) new_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(dyn, new_cap * sizeof(seq_t));
                if (!tmp) return 0;  /* allocation failed; caller may handle */
                dyn = tmp;
                dyn_cap = new_cap;
                /* Initialise new entries with zeros */
                for (size_t i = dyn_computed; i < dyn_cap; ++i)
                    dyn[i] = 0;
            }

            /* Copy initial known values into dynamic array if not already */
            if (!dyn_computed) {
                for (size_t i = 0; i < 24; ++i) dyn[i] = init_vals[i];
                dyn_computed = 24;
            }

            for (size_t i = dyn_computed; i <= (size_t)n; ++i)
                dyn[i] = (seq_t)0;   /* placeholder; will be set below */

            /* Compute recurrence */
            for (size_t i = dyn_computed; i <= (size_t)n; ++i) {
                long long sum = 0;
                sum += (long long)dyn[i-2];
                sum -= (long long)dyn[i-4];
                sum += (long long)dyn[i-6];
                sum += (long long)dyn[i-7];
                sum -= (long long)dyn[i-8];
                sum -= (long long)dyn[i-9];
                sum += (long long)dyn[i-10];
                sum += (long long)dyn[i-11];
                sum -= (long long)dyn[i-13];
                sum += (long long)dyn[i-15];
                sum -= (long long)dyn[i-17];
                dyn[i] = (seq_t)sum;
            }
            dyn_computed = (size_t)n + 1;
            return dyn[(size_t)n];
        }

        /* Compute using static array */
        for (size_t i = computed; i <= (size_t)n; ++i) {
            long long sum = 0;
            sum += (long long)cache[i-2];
            sum -= (long long)cache[i-4];
            sum += (long long)cache[i-6];
            sum += (long long)cache[i-7];
            sum -= (long long)cache[i-8];
            sum -= (long long)cache[i-9];
            sum += (long long)cache[i-10];
            sum += (long long)cache[i-11];
            sum -= (long long)cache[i-13];
            sum += (long long)cache[i-15];
            sum -= (long long)cache[i-17];
            cache[i] = (seq_t)sum;
        }
        computed = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
