/*
 * A272637  a(1)=1, a(2)=83; thereafter a(n) = squarefree part of
 *          a(n-1)+a(n-2).
 * https://oeis.org/A272637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A272637:
   a(1) = 1, a(2) = 83;
   for n >= 3, a(n) = squarefree part of a(n-1)+a(n-2)
   (squarefree part = product of the odd‑power prime factors).  */
#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;
static size_t cache_size = 0;

/* Return the squarefree part of v:
   for v = ∏ p_i^{e_i}, result = ∏ p_i^{e_i mod 2}. */
static seq_t squarefree_part(seq_t v)
{
    seq_t n = v;
    seq_t res = 1;
    seq_t p = 2;
    while (p <= n / p)
    {
        if (n % p == 0)
        {
            int parity = 0;
            do
            {
                n /= p;
                parity ^= 1;
            } while (n % p == 0);
            if (parity)
                res *= p;
        }
        p = (p == 2) ? 3 : p + 2;   /* skip even numbers after 2 */
    }
    if (n > 1)            /* remaining prime with odd exponent */
        res *= n;
    return res;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)83;

    /* Grow cache if needed.  cache[k] holds a(k+1). */
    size_t needed = (size_t)n;
    if (cache_size < needed)
    {
        seq_t *new_cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!new_cache)
            return 0;        /* allocation failure – unlikely in tests */
        cache = new_cache;
        if (cache_size == 0)
        {
            cache[0] = (seq_t)1;
            cache[1] = (seq_t)83;
            cache_size = 2;
        }
        for (size_t i = cache_size; i < needed; ++i)
        {
            /* i corresponds to a(i+1).  Use 0‑based indices. */
            seq_t sum = cache[i - 1] + cache[i - 2];
            cache[i] = squarefree_part(sum);
        }
        cache_size = needed;
    }
    return cache[n - 1];
}
