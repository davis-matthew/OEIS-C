/*
 * A320062  Nonprimes with odd digits only.
 * https://oeis.org/A320062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth nonprime number whose decimal representation *
 *            contains only odd digits. * * The implementation scans
 *            successive integers, keeps a static cache of * found terms, and
 *            stops when the requested index is reached. It does * not use
 *            dynamic allocation of seq_t objects, so it works both f...
 * Range:     u8 n <= 28, u16 n <= 2087, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime.h"

static seq_t cache[50000];          /* enough for n up to ~10⁴ */
static size_t cache_len = 0;
static idx_t last_tested = 0;

/* Return true if all decimal digits of x are odd. */
static inline bool all_odd_digits(idx_t x)
{
    do
    {
        uint8_t d = x % 10;
        if ((d & 1) == 0)  /* even digit found */
            return false;
        x /= 10;
    } while (x);
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;       /* sequence is defined for n≥1 */

    if (n <= cache_len)
        return cache[n - 1];

    for (idx_t x = last_tested + 1; cache_len < (size_t)n; ++x)
    {
        last_tested = x;

        if (!all_odd_digits(x))
            continue;

        if (is_prime(x))
            continue;          /* skip primes */

        /* store the discovered nonprime */
        /* assume cache array is large enough for all required terms */
        cache[cache_len++] = (seq_t)x;
    }

    return cache[n - 1];
}
