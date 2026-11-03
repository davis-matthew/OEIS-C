/*
 * A255165  a(n) = Sum_{k=2..n} floor(log(n)/log(k)), n >= 1.
 * https://oeis.org/A255165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..⌊log2(n)⌋} (⌊n^(1/i)⌋ - 1)
 * Range:     u8 n <= 232, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 9953 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Helper: test whether base^exp <= limit using 128‑bit arithmetic. */
static int pow_le(idx_t base, idx_t exp, idx_t limit)
{
    unsigned __int128 prod = 1;
    for (idx_t i = 0; i < exp; ++i) {
        prod *= (unsigned __int128)base;
        if (prod > (unsigned __int128)limit)
            return 0; /* limit exceeded */
    }
    return 1; /* prod <= limit */
}

/* Helper: floor of the integer i‑th root of n (i ≥ 1). */
static idx_t floor_root_idx(idx_t n, idx_t i)
{
    idx_t lo = 1, hi = n;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;   /* upper mid to avoid infinite loop */
        if (pow_le(mid, i, n))
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* maximum exponent i such that 2^i ≤ n */
    idx_t max_i = 0;
    if (n > 0)
        max_i = 63 - __builtin_clzll(n);

    seq_t sum = 0;
    for (idx_t i = 1; i <= max_i; ++i) {
        idx_t root = floor_root_idx(n, i);
        sum += (seq_t)(root - 1);
    }
    return sum;
}
