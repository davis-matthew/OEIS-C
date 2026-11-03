/*
 * A347240  a(n) is the largest prime factor (A006530) of all terms
 *          encountered when iterating the map x -> A000593(x), when starting
 *          from x = n, but excluding the n itself. If n is a power of 2, then
 *          a(n) = 1. If 1 is never reached, then a(n) = -1.
 * https://oeis.org/A347240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest prime factor found while iterating the map * x →
 *            A000593(x) (sum of odd divisors) from the starting value n *
 *            (excluding n itself). Return 1 if n is a power of two; otherwise
 *            * return –1 if the iteration never reaches 1
 * Range:     u8 n <= 64, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 113 terms through n = 8279 (2026-11-01)
 * Helpers:   ipow_i32, is_pow2, largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/ipow_i32.h"
#include "../utils/is_pow2.h"
#include "../utils/largest_prime_factor.h"

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

/* compute σ₀'(n): sum of odd divisors of n */
static seq_t sum_of_odd_divisors(seq_t n)
{
    /* remove all factors of 2 */
    while ((n & 1) == 0) n >>= 1;
    seq_t result = 1;
    if (n == 1) return 1;

    seq_t root = floor_sqrt(n);
    for (seq_t d = 3; d <= root; d += 2) {
        if (n % d == 0) {
            seq_t sum = 1;
            seq_t power = 1;
            while (n % d == 0) {
                n /= d;
                power *= d;
                sum += power;
            }
            result *= sum;
            root = floor_sqrt(n);
        }
    }
    if (n > 1) {               /* remaining odd prime factor */
        result *= (1 + n);
    }
    return result;
}

seq_t a(idx_t n)
{
    /* n is a power of two → a(n)=1 */
    if (is_pow2((seq_t)n)) return 1;

    seq_t cur = (seq_t)n;
    bool reached_one = false;
    bool found = false;
    seq_t gpf_max = 0;

    /* visited values to detect loops without reaching 1 */
    seq_t visited[2000];
    size_t vlen = 0;

    while (1) {
        seq_t nxt = sum_of_odd_divisors(cur);
        if (nxt == 1) { reached_one = true; break; }

        /* update maximum largest prime factor */
        if (nxt > 1) {
            uint64_t gpf = largest_prime_factor((uint64_t)nxt);
            if (!found || gpf > (uint64_t)gpf_max) {
                gpf_max = (seq_t)gpf;
                found = true;
            }
        }

        /* cycle detection */
        bool seen = false;
        for (size_t i = 0; i < vlen; ++i) {
            if (visited[i] == nxt) { seen = true; break; }
        }
        if (seen) break;                /* loop without reaching 1 */
        if (vlen < sizeof(visited)/sizeof(visited[0]))
            visited[vlen++] = nxt;
        cur = nxt;
    }

    if (reached_one) return found ? gpf_max : (seq_t)1;
    return (seq_t)(-1);                 /* sentinel for “never reaches 1” */
}
