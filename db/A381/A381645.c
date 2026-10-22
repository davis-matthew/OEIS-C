/*
 * A381645  a(n) is the largest integer with distinct digits whose digital sum
 *          is n.
 * https://oeis.org/A381645
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = the largest integer with distinct digits and digit sum n
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 44, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Cache of the pre‑computed values for digit sums 0 … 45. */
static uint64_t _a_cache[46];
static bool _a_initialized = false;

seq_t a(idx_t n)
{
    /* Lazily fill the cache on first use. */
    if (!_a_initialized) {
        /* Set all entries to 0. */
        for (int i = 0; i < 46; ++i)
            _a_cache[i] = 0;

        /* Enumerate all subsets of {0,…,9}. */
        for (idx_t mask = 0; mask < (1ull << 10); ++mask) {
            unsigned sum = 0;
            uint64_t num = 0;

            /* Build the number from the most significant digit to the least. */
            for (int d = 9; d >= 0; --d) {
                if (mask & (1u << d)) {
                    sum += (unsigned)d;
                    num = num * 10 + (uint64_t)d;
                }
            }

            if (sum <= 45u) {
                /* Keep the largest number for this digit sum. */
                if (_a_cache[sum] == 0 || num > _a_cache[sum])
                    _a_cache[sum] = num;
            }
        }
        _a_initialized = true;
    }

    /* Return the cached value if n is in range, otherwise 0. */
    if (n <= 45u)
        return (seq_t)_a_cache[(size_t)n];
    return (seq_t)0;
}
