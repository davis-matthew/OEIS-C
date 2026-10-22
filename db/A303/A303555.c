/*
 * A303555  Triangle read by rows: T(n,k) = 2^(n-k)*prime(k)#, 1 <= k <= n,
 *          where prime(k)# is the product of first k primes.
 * https://oeis.org/A303555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 27, u32 n <= 54, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A303555
   Triangle read by rows where T(n,k) = 2^(n-k) * product of first k primes.
   The returned sequence is the flattened triangle, i.e.
   a(n) = T(r,k) with r the smallest integer such that r(r+1)/2 >= n
   and k = n - r(r-1)/2. */

/* Labeled includes for helper functions */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"

static uint32_t primes[2000];   /* buffer for first primes, enough for any n <= 10^8 */
static size_t  primes_cnt = 0;  /* number of primes stored */

/* Ensure that at least k primes are stored in primes[0..k-1] */
static void ensure_primes(size_t k)
{
    if (primes_cnt >= k) return;

    /* Start generating primes if none stored yet */
    if (primes_cnt == 0) {
        primes[0] = 2;
        primes_cnt = 1;
    }

    uint32_t candidate = primes[primes_cnt - 1] + 1;
    while (primes_cnt < k) {
        bool prime = true;
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint32_t p = primes[i];
            if ((uint64_t)p * p > candidate) break;
            if (candidate % p == 0) { prime = false; break; }
        }
        if (prime) {
            primes[primes_cnt++] = candidate;
        }
        ++candidate;
    }
}

seq_t a(idx_t n)
{
    /* find row r and column k such that row r contains n */
    idx_t r = 1;
    idx_t cum = 0;            /* cumulative count of previous rows: t(r-1) */
    while (cum + r < n) {
        cum += r;
        ++r;
    }
    idx_t k = n - cum;        /* 1 <= k <= r */

    /* Ensure we have enough primes for this k */
    ensure_primes((size_t)k);

    /* product of first k primes */
    seq_t prod = 1;
    for (size_t i = 0; i < k; ++i) {
        prod *= (seq_t)primes[i];
    }

    /* multiply by 2^(r-k) */
    seq_t result = prod * pow2(r - k);
    return result;
}
