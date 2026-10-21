/*
 * A243910  Least number k>0 such that 3^k contains exactly n different
 *          digits.
 * https://oeis.org/A243910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, full, fini
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 7, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A243910:  a(n) = smallest k > 0 such that 3^k contains exactly n distinct decimal digits.
   Computed by iterating powers of 3 until all possible digit counts (1‑10) are found. */

#include <stdbool.h>
#include "../utils/digit_counts.h"

static const idx_t MAX_N = 10;          /* maximum distinct digits possible */
static seq_t cache[11];                /* cache[ n ] stores result for a(n), n∈[1,10] */
static bool have[11];                  /* have[ n ] indicates if cache[ n ] is valid */
static bool initialized = false;

/* Fill cache with the smallest exponent for each digit count from 1 to 10. */
static void initialize(void)
{
    if (initialized)
        return;

    for (idx_t i = 0; i <= MAX_N; ++i) {
        have[i] = false;
    }

    seq_t power = 3;          /* 3^1 */
    idx_t k = 1;              /* current exponent */
    idx_t found = 0;          /* number of distinct counts found */

    while (found < MAX_N) {
        unsigned char cnt[10] = {0};
        digit_counts(power, cnt);

        idx_t distinct = 0;
        for (idx_t d = 0; d < 10; ++d)
            if (cnt[d] != 0)
                ++distinct;

        if (distinct <= MAX_N && !have[distinct]) {
            cache[distinct] = (seq_t)k;
            have[distinct]  = true;
            ++found;
        }

        /* prepare next power of 3 */
        power = power * (seq_t)3;
        ++k;
    }
    initialized = true;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                /* n must be ≥1; return 0 for safety */

    if (n > MAX_N)
        return (seq_t)0;                /* no power of 3 has more than 10 distinct digits */

    if (!initialized)
        initialize();

    return cache[n];
}
