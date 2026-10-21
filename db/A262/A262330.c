/*
 * A262330  Number of (n+1) X (6+1) 0..1 arrays with each row and column
 *          divisible by 3, read as a binary number with top and left being
 *          the most significant bits.
 * https://oeis.org/A262330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the following recurrence for n ≥ 16 a(n) =
 *            43*a(n-1) + 793*a(n-2) - 34099*a(n-3) - 191667*a(n-4) +
 *            8241681*a(n-5) + 20015927*a(n-6) - 860684861*a(n-7) -
 *            980560940*a(n-8) + 42164120420*a(n-9) + 22309346640*a(n-10) -
 *            959301905520*a(n-11) - 209959392832*a(n-12) +
 *            9028253891776*a(n-13)...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/seq_from_str.h"

static seq_t seq_const[15];
static bool seq_const_ready = false;
static const char *seq_const_str[15] = {
    "43", "261", "10935", "252097", "10789339", "394241389",
    "16940254423", "699094613961", "30056993215803",
    "1279198648576981", "55003871225837815", "2359070696262035857",
    "101439297012081117979", "4359072407628680510589", "187439773845177573589143"
};

/* Initialised on first use.  */
static void init_seq_consts(void)
{
    if (seq_const_ready) return;
    seq_const_ready = true;
    for (int i = 0; i < 15; ++i)
        seq_const[i] = seq_from_str(seq_const_str[i]);
}

/* Recurrence coefficients.  Signed values are used; it is assumed that
   `seq_t` supports signed arithmetic (true for the arbitrary-precision
   build).  For builds where `seq_t` is unsigned the values are only
   accessed for n ≤ 15, which are never larger than 2⁶⁴-1. */
static const long long coeff[15] = {
    43, 793, -34099, -191667, 8241681, 20015927,
    -860684861, -980560940, 42164120420, 22309346640,
    -959301905520, -209959392832, 9028253891776,
    540712883200, -23250653977600
};

/* Static cache for the recurrence.  */
static seq_t cache[15];
static idx_t cache_n = 0;          /* Last n for which cache is valid. */
static bool cache_ready = false;

/* Initialise cache with the first 15 terms.  */
static void init_cache(void)
{
    if (cache_ready) return;
    init_seq_consts();
    for (int i = 0; i < 15; ++i)
        cache[i] = seq_const[i];
    cache_n = 15;
    cache_ready = true;
}

seq_t a(idx_t n)
{
    init_seq_consts();

    /* Return initial values directly.  */
    if (n <= 15)
    {
        return seq_const[n - 1];
    }

    /* Initialise or extend the cache as needed.  */
    init_cache();
    while (cache_n < n)
    {
        /* Compute the next term using the recurrence.  */
        seq_t sum = 0;
        for (int k = 1; k <= 15; ++k)
        {
            seq_t prod = cache[15 - k] * (seq_t)coeff[k - 1];
            sum += prod;
        }

        /* Shift the cache and store the new value.  */
        for (int i = 0; i < 14; ++i)
            cache[i] = cache[i + 1];
        cache[14] = sum;
        ++cache_n;
    }
    return cache[14];
}
