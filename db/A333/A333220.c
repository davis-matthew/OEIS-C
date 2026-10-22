/*
 * A333220  The number k such that the k-th composition in standard order
 *          consists of the prime indices of n in weakly increasing order.
 * https://oeis.org/A333220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the index k such that the k‑th composition in standard
 *            order consists of the prime indices of n in weakly increasing
 *            order
 * Range:     u8 n <= 22, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   build_primes_upto, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Cache of primes up to the largest n handled so far. */
    static uint64_t *primes = NULL;
    static size_t prime_cnt = 0;
    static idx_t prime_limit = 0;

    if (n > prime_limit)
    {
        build_primes_upto(n, &primes, &prime_cnt);
        prime_limit = n;
    }

    /* Factor n; store the 1‑based indices of its prime factors
       (with multiplicity) in a small stack array. */
    idx_t remaining = n;
    seq_t indices[64] = {0};
    size_t idx_len = 0;

    for (size_t i = 0;
         i < prime_cnt && primes[i] <= remaining / primes[i];
         ++i)
    {
        uint64_t p = primes[i];
        if (remaining % p == 0)
        {
            seq_t idx = (seq_t)(i + 1);          /* 1‑based index of p */
            while (remaining % p == 0)
            {
                indices[idx_len++] = idx;
                remaining /= p;
            }
        }
    }
    if (remaining > 1)                         /* last prime factor */
    {
        /* Find its index in the prime list. */
        size_t pos = 0;
        while (pos < prime_cnt && primes[pos] < (uint64_t)remaining) ++pos;
        if (pos < prime_cnt && primes[pos] == (uint64_t)remaining)
        {
            seq_t idx = (seq_t)(pos + 1);
            indices[idx_len++] = idx;
        }
    }

    /* Convert the (reversed) composition to its index k. */
    seq_t k      = 0;
    seq_t pos_sum = 0;
    for (size_t i = idx_len; i > 0; --i)
    {
        seq_t part = indices[i - 1];
        pos_sum += part;
        k += pow2((idx_t)(pos_sum - 1));
    }

    return k;
}
