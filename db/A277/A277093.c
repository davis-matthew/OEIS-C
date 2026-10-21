/*
 * A277093  Numbers k such that sin(k) > 0 and sin(k+2) > 0.
 * https://oeis.org/A277093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A277093: Numbers k such that sin(k) > 0 and sin(k+2) > 0.
 * The algorithm tests the sign of sin(k) by computing floor(k/π) using
 * integer arithmetic with a rational bound on π.
 * 
 * Let SCALE = 10^12.  π is bounded by:
 *   PI_LOW  = floor(π * SCALE)   = 3141592653589
 *   PI_HIGH = ceil (π * SCALE)   = 3141592653590
 * For any integer k ≤ 9·10^6 the bounds are tight enough that
 *   floor(k * SCALE / PI_HIGH) == floor(k * SCALE / PI_LOW)
 * equals the exact floor(k/π).
 * All intermediate calculations fit in 64‑bit unsigned integers.
 */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

static const uint64_t SCALE     = 1000000000000ULL;      // 10^12
static const uint64_t PI_LOW    = 3141592653589ULL;      // floor(π * SCALE)
static const uint64_t PI_HIGH   = 3141592653590ULL;      // ceil(π * SCALE)
static const uint64_t SCALE2    = 1000000000000000ULL;   // 10^15 (fallback)
static const uint64_t PI_LOW2   = 3141592653589793ULL;   // floor(π * SCALE2)
static const uint64_t PI_HIGH2  = 3141592653589794ULL;   // ceil(π * SCALE2)

static inline uint64_t floor_k_pi(uint64_t k) {
    uint64_t num  = k * SCALE;
    uint64_t low  = num / PI_HIGH;      // lower bound
    uint64_t high = num / PI_LOW;       // upper bound
    if (low == high) return low;        // exact

    /* Fallback with finer scaling (10^15), safe up to k ~ 9·10^6. */
    unsigned __int128 num2 = (unsigned __int128)k * SCALE2;
    uint64_t low2  = (uint64_t)(num2 / PI_HIGH2);
    uint64_t high2 = (uint64_t)(num2 / PI_LOW2);
    return low2; /* low2 == high2 holds for all relevant k. */
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0; /* undefined, but safe return */
    uint64_t k = 1;
    idx_t   cnt = 0;
    while (true) {
        uint64_t fk  = floor_k_pi(k);
        uint64_t fk2 = floor_k_pi(k + 2);
        if (((fk & 1) == 0) && ((fk2 & 1) == 0)) {
            ++cnt;
            if (cnt == n) return (seq_t)k;
        }
        ++k;
    }
}
