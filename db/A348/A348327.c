/*
 * A348327  Characteristic function of numbers that have no middle divisors.
 * https://oeis.org/A348327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n has no middle divisors, 0 otherwise. A middle
 *            divisor d satisfies √(n/2) ≤ d ≤ √(2n) (integers d dividing n).
 *            This function checks the interval for any divisor of n
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 8546 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* ------------- Helper: floor sqrt of 2n (2⋅n) -------------- */
static inline idx_t floor_sqrt_2n(idx_t n)
{
    __int128 two_n = (__int128)2 * ( __int128)n;          /* 2n, fits in 128 bits */
    idx_t low  = 0;
    idx_t high = ((idx_t)1u << 33) - 1;                  /* √(2⋅2⁶⁴) < 2³³ */

    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if ((__int128)mid * ( __int128)mid <= two_n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;        /* floor_sqrt(2n) */
}

/* ------------- Helper: ceil sqrt of n/2 (smallest d with 2d² ≥ n) -------------- */
static inline idx_t ceil_sqrt_half_n(idx_t n)
{
    if (n == 0)
        return 0;

    idx_t high = floor_sqrt_2n(n);          /* upper bound for d */
    idx_t low  = 1;

    while (low < high) {
        idx_t mid = (low + high) >> 1;
        if ((__int128)mid * ( __int128)mid * 2 >= (__int128)n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;        /* ceil_sqrt(n/2) */
}

/* ------------------------- Main sequence function ---------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)          /* not used (offset is 1) but harmless */
        return (seq_t)0;

    idx_t high = floor_sqrt_2n(n);          /* maximal d with d² ≤ 2n */
    idx_t low  = ceil_sqrt_half_n(n);       /* minimal d with 2d² ≥ n */

    if (low > high)
        return (seq_t)1;                    /* empty interval → no middle divisor */

    for (idx_t d = low; d <= high; ++d) {
        if (n % d == 0)
            return (seq_t)0;                /* found a middle divisor */
    }
    return (seq_t)1;                        /* none found */
}
