/*
 * A391497  Biquadratefree numbers that are neither cubefree nor cubefull.
 * https://oeis.org/A391497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th integer that is biquadratefree (no prime exponent
 *            >3), contains at least one prime factor with exponent exactly 3,
 *            and contains at least one prime factor with exponent 1 or 2 (but
 *            not all exponents equal 3). This is equivalent to the OEIS
 *            definition
 * Range:     u8 n <= 19, u16 n <= 6006, u32 n <= 9751, u64 n <= 9751,
 *            u128 n <= 9751, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t *cache = NULL;      /* array of found terms */
static size_t cache_len = 0;     /* number of terms stored */
static size_t cache_cap = 0;     /* allocated capacity */
static uint64_t last_checked = 1;/* current integer to test */

/* Check whether x satisfies the conditions for the sequence. */
static bool good_number(uint64_t x)
{
    bool has3   = false; /* at least one exponent == 3 */
    bool has_less = false; /* at least one exponent < 3 */

    uint64_t n = x;
    for (uint64_t p = 2; p * p <= n; ++p)
    {
        if (n % p == 0)
        {
            unsigned e = 0;
            while (n % p == 0)
            {
                n /= p;
                ++e;
                if (e > 3) return false;    /* biquadratefree violated */
            }
            if (e == 3)    has3 = true;
            else if (e < 3) has_less = true;
        }
    }
    if (n > 1)          /* remaining prime factor with exponent 1 */
        has_less = true;

    return has3 && has_less;
}

/* Ensure the cache can hold 'needed' elements. */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < needed) new_cap = needed;
    void *tmp = realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* allocation failure */
    cache = (seq_t *)tmp;       /* cast to seq_t* is allowed in C++ */
    cache_cap = new_cap;
}

/* Return the nth term (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* out of bounds, return 0 */

    while (cache_len < (size_t)n)
    {
        if (good_number(last_checked))
        {
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)last_checked; /* implicit ctor OK  */
        }
        ++last_checked;
    }
    return cache[n - 1];
}
