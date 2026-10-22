/*
 * A365472  Numbers whose digits are either all primes or all nonprimes.
 * https://oeis.org/A365472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   all_nonprime_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: generate integers whose digits are either all primes
   (2,3,5,7) or all non‑primes (0,1,4,6,8,9) by scanning the natural
   numbers in increasing order until the requested rank is reached. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/all_nonprime_digits.h"

/* static cache to speed up arbitrary order of calls */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t last_checked = 0;   /* first number not yet examined */

/* ensure the cache can hold at least 'needed' elements */
static void grow_cache(size_t needed)
{
    if (cache_cap < needed)
    {
        size_t new_cap = cache_cap == 0 ? 64 : cache_cap * 2;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* check whether all digits of x are prime (2,3,5,7) */
static bool all_prime_digits(seq_t x)
{
    if (x == 0) return false;  /* 0 is not a prime digit */
    seq_t t = x;
    while (t > 0)
    {
        seq_t d = t % 10;
        if (!(d == 2 || d == 3 || d == 5 || d == 7)) return false;
        t /= 10;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for offset < 1, safe return */

    /* extend cache until we have at least n terms */
    if (cache_len < (size_t)n)
    {
        grow_cache((size_t)n);
        seq_t num = last_checked;
        while (cache_len < (size_t)n)
        {
            if (all_prime_digits(num) || all_nonprime_digits(num))
                cache[cache_len++] = num;
            num++;
        }
        last_checked = num;
    }
    return cache[(size_t)n - 1];
}
