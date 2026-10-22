/*
 * A308760  Sum of the largest parts of the partitions of n into 4 parts.
 * https://oeis.org/A308760
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A308760 – Sum of the largest parts of the partitions of n into 4 parts.
    This implementation uses the linear recurrence
        a(n) = a(n-2) + 2a(n-3) + 2a(n-4) – 2a(n-5) – 3a(n-6) – 4a(n-7)
              + 4a(n-9) + 3a(n-10) + 2a(n-11) – 2a(n-12) – 2a(n-13) – a(n-14)
              + a(n-16)      for n > 15,
    with the initial values
        a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=1, a(5)=2, a(6)=5,
        a(7)=9, a(8)=17, a(9)=25, a(10)=41, a(11)=57, a(12)=84,
        a(13)=112, a(14)=154, a(15)=197.
    The values are cached so that arbitrary call order runs in linear time in
    the maximum required n.
*/

#include <stdlib.h>
#include <stdint.h>

/* static cache of computed values */
static seq_t *cache = NULL;
static idx_t  cache_cap = 0;     /* current capacity of cache */
static idx_t  max_calc  = 0;     /* largest n already computed */

/* initialise the cache up to at least n */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_cap)
        return;

    idx_t new_cap = cache_cap * 2;
    if (new_cap < n + 1)
        new_cap = n + 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort(); /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* compute sequence values up to n (inclusive) */
static void compute_up_to(idx_t n)
{
    for (idx_t i = max_calc + 1; i <= n; ++i) {
        if (i < 16) {
            /* initial values – these are all ≤ 197 and fit in every seq_t */
            static const seq_t init_vals[16] = {
                0ULL, 0ULL, 0ULL, 0ULL, 1ULL, 2ULL, 5ULL, 9ULL,
                17ULL, 25ULL, 41ULL, 57ULL, 84ULL, 112ULL, 154ULL, 197ULL
            };
            cache[i] = init_vals[i];
        } else {
            /* use recurrence with a signed 128‑bit temporary to avoid
               unsigned wrap‑around */
            __int128 total = 0;
            total += (__int128)cache[i-2];
            total += 2 * (__int128)cache[i-3];
            total += 2 * (__int128)cache[i-4];
            total -= 2 * (__int128)cache[i-5];
            total -= 3 * (__int128)cache[i-6];
            total -= 4 * (__int128)cache[i-7];
            total += 4 * (__int128)cache[i-9];
            total += 3 * (__int128)cache[i-10];
            total += 2 * (__int128)cache[i-11];
            total -= 2 * (__int128)cache[i-12];
            total -= 2 * (__int128)cache[i-13];
            total -= (__int128)cache[i-14];
            total += (__int128)cache[i-16];
            cache[i] = (seq_t)total;
        }
    }
    max_calc = n;
}

/* public function required by the framework */
seq_t a(idx_t n)
{
    if (n < 0)                 /* keep interface stable even for negative n */
        return 0;
    ensure_cache(n);
    if (n > max_calc)
        compute_up_to(n);
    return cache[n];
}
