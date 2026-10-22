/*
 * A326889  a(1) = 1; thereafter a(n) = a(n-1) / phi(n) if phi(n) divides
 *          a(n-1), otherwise a(n) = a(n-1) * phi(n), where phi is the Euler
 *          phi-function A000010.
 * https://oeis.org/A326889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; For n > 1: a(n) = a(n-1) / phi(n) if phi(n) divides
 *            a(n-1) a(n) = a(n-1) * phi(n) otherwise
 * Range:     u8 n <= 10, u16 n <= 23, u32 n <= 87, u64 n <= 179,
 *            u128 n <= 330, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Euler totient function implemented by trial division */
static seq_t my_phi(idx_t m)
{
    seq_t result = (seq_t)m;          /* result holds the current value of phi   */
    uint64_t temp = (uint64_t)m;      /* working copy for factorisation         */

    for (uint64_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            while (temp % p == 0) temp /= p;
            result -= result / (seq_t)p;   /* result -= result / p   */
        }
    }
    if (temp > 1)                       /* last prime factor > 1     */
    {
        result -= result / (seq_t)temp;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n < 1, return 0 */

    /* Dynamic cache for computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;      /* highest index that has been computed */

    /* Allocate or grow the cache if necessary */
    if (cache == NULL)
    {
#ifdef __cplusplus
        cache = new seq_t[n + 2];      /* add room for index 0 and n          */
#else
        cache = (seq_t *)malloc((n + 2) * sizeof(seq_t));
#endif
        if (!cache) abort();
        cache_size = 0;
    }
    else if (n > cache_size)
    {
        /* Allocate a larger array and copy old values */
#ifdef __cplusplus
        seq_t *tmp = new seq_t[n + 2];
#else
        seq_t *tmp = (seq_t *)malloc((n + 2) * sizeof(seq_t));
#endif
        if (!tmp) abort();
        for (idx_t i = 1; i <= cache_size; ++i) tmp[i] = cache[i];
#ifdef __cplusplus
        delete[] cache;
#else
        free(cache);
#endif
        cache = tmp;
    }

    /* Compute missing values up to index n */
    for (idx_t i = cache_size + 1; i <= n; ++i)
    {
        if (i == 1)
        {
            cache[1] = (seq_t)1;          /* initial value */
        }
        else
        {
            seq_t phi_i = my_phi(i);
            seq_t prev   = cache[i - 1];
            if (prev % phi_i == 0)
                cache[i] = prev / phi_i;
            else
                cache[i] = prev * phi_i;
        }
    }
    cache_size = n;

    return cache[n];
}
