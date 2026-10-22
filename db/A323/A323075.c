/*
 * A323075  The fixed point reached when map x -> 1+(x-(largest divisor d <
 *          x)) is iterated, starting from x = n.
 * https://oeis.org/A323075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 9557, u32 n <= 9557, u64 n <= 9557,
 *            u128 n <= 9557, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A323075 – Fixed point of  x → 1 + (x – largest divisor < x)
   Computation: starting from n, repeatedly apply the map until a prime
   (fixed point) is reached.  The map reduces a composite x to
   1 + (x – x / spf(x)) where spf(x) is the smallest prime factor of x.
   If x is 1, the fixed point is 1.  Each iteration decreases x,
   so the algorithm terminates quickly.
   No memoisation is required; the routine runs in O(√n) time
   (bounded by the trial‑division checks). */

#include <stdint.h>
#include <stdbool.h>
#include <limits.h>

static inline bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2ULL == 0ULL) return n == 2ULL;
    for (uint64_t d = 3ULL; d * d <= n; d += 2ULL) {
        if (n % d == 0ULL) return false;
    }
    return true;
}

static inline uint64_t smallest_prime_factor_uint64(uint64_t n)
{
    if (n < 2) return n;
    if (n % 2ULL == 0ULL) return 2ULL;
    for (uint64_t d = 3ULL; d * d <= n; d += 2ULL) {
        if (n % d == 0ULL) return d;
    }
    return n;               /* n is prime */
}

/* External function required by the test harness */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* out of range, not used by tests */

    uint64_t cur = (uint64_t)n;

    if (cur == 1) return (seq_t)1; /* fixed point for n = 1 */

    while (true) {
        if (is_prime_uint64(cur) || cur == 1) {
            return (seq_t)cur;    /* prime (or 1) is the fixed point */
        }
        uint64_t spf = smallest_prime_factor_uint64(cur);
        /* for a composite cur, spf < cur, so cur / spf >= 2 */
        cur = 1ULL + (cur - cur / spf);
    }
}
