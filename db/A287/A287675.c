/*
 * A287675  Positions of 0 in A287674; complement of A287676.
 * https://oeis.org/A287675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2k-1) = floor(k*phi)+3k-2, a(2k) = floor(k*phi)+3k-1, where k
 *            = ceil(n/2)
 * Range:     u8 n <= 6, u16 n <= 61, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9371 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

static seq_t floor_k_phi(idx_t k)
{
    /* floor(k*phi) with phi = (1+sqrt5)/2 */
    seq_t k_t = (seq_t)k;
    seq_t t = floor_sqrt(k_t * k_t * 5);
    return (k_t + t) / 2;
}

seq_t a(idx_t n)
{
    /* n >= 1 (offset = 1) */
    if (n & 1) /* odd */
    {
        idx_t k = n >> 1;          /* (n-1)/2 */
        k++;                       /* ceil(n/2) */
        seq_t val = floor_k_phi(k);
        val += 3 * (seq_t)k;
        val -= 2;
        return val;
    }
    else /* even */
    {
        idx_t k = n >> 1;          /* n/2 */
        seq_t val = floor_k_phi(k);
        val += 3 * (seq_t)k;
        val -= 1;
        return val;
    }
}
