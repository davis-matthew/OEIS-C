/*
 * A335881  a(n) = max(A329697(n), A331410(n)).
 * https://oeis.org/A335881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max(A329697(n), A331410(n)); A329697 iterates k -> k-k/p
 *            and A331410 iterates k -> k+k/p (p = largest prime factor of k),
 *            each counting steps until a power of 2 is reached
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9736 (2026-11-08)
 * Helpers:   largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/largest_prime_factor.h"

static bool A335881_pow2(uint64_t k)
{
    return k != 0 && (k & (k - 1)) == 0;
}

static uint64_t A335881_steps(uint64_t k, bool up)
{
    uint64_t cnt = 0;
    while (!A335881_pow2(k)) {
        uint64_t p = largest_prime_factor(k);
        if (up)
            k += k / p;
        else
            k -= k / p;
        ++cnt;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    uint64_t x = (uint64_t)n;
    uint64_t d = A335881_steps(x, false);
    uint64_t u = A335881_steps(x, true);
    return (seq_t)(d > u ? d : u);
}
