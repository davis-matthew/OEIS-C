/*
 * A366717  Smallest prime dividing 12^n - 1.
 * https://oeis.org/A366717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = smallest prime factor of (12^n - 1) The sequence is
 *            periodic with period 12
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Validated: partial, pass, 71 terms through n = 71 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Compute the smallest prime factor of a 64‑bit integer.
   (All values needed here are ≤ 12^12 - 1 < 9·10^12.) */
static uint64_t spf64(uint64_t n)
{
    /* Even numbers */
    if ((n & 1ULL) == 0ULL) return 2ULL;
    for (uint64_t d = 3ULL; d * d <= n; d += 2ULL)
    {
        if (n % d == 0ULL) return d;
    }
    return n; /* n is prime */
}

/* Precomputed array of a(1) … a(12) */
static seq_t small_prime[13];
static bool initialized = false;

static void init(void)
{
    for (int k = 1; k <= 12; ++k)
    {
        uint64_t power = 1ULL;
        for (int i = 0; i < k; ++i)
            power *= 12ULL;
        uint64_t val = power - 1ULL;
        uint64_t sp = spf64(val);
        small_prime[k] = (seq_t)sp;
    }
    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized) init();

    idx_t r = n % 12ULL;
    if (r == 0) r = 12;
    return small_prime[r];
}
