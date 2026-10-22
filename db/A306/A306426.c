/*
 * A306426  Sum of the smallest side lengths of all integer-sided triangles
 *          with perimeter n whose largest side length is prime.
 * https://oeis.org/A306426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{p prime, 2p < n} [
 *            sum_{a=max(1,n-2p)}^{floor((n-p)/2)} a ]
 * Range:     u8 n <= 48, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This computes the sum of the smallest side lengths of all integer‑sided triangles
   with perimeter n whose largest side length is a prime. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PRIME_LIMIT 200000   /* large enough for n up to ~10^5 */

static unsigned char prime_is_composite[MAX_PRIME_LIMIT + 1];
static uint32_t prime_list[MAX_PRIME_LIMIT / 10 + 10];
static size_t prime_count = 0;
static bool primes_initialized = false;

static void init_primes(void)
{
    if (primes_initialized) return;
    memset(prime_is_composite, 0, sizeof(prime_is_composite));
    prime_is_composite[0] = prime_is_composite[1] = 1;

    for (size_t i = 2; i * i <= MAX_PRIME_LIMIT; ++i)
    {
        if (!prime_is_composite[i])
        {
            for (size_t j = i * i; j <= MAX_PRIME_LIMIT; j += i)
                prime_is_composite[j] = 1;
        }
    }

    for (size_t i = 2; i <= MAX_PRIME_LIMIT; ++i)
    {
        if (!prime_is_composite[i])
            prime_list[prime_count++] = (uint32_t)i;
    }
    primes_initialized = true;
}

seq_t a(idx_t n)
{
    init_primes();

    seq_t total = 0;

    if (n < 5) return 0;          /* for n <= 4 there is no valid triangle */

    idx_t limit = (n - 1) / 2;    /* largest prime p must satisfy 2p < n */

    for (size_t pi = 0; pi < prime_count && prime_list[pi] <= limit; ++pi)
    {
        idx_t p   = prime_list[pi];
        idx_t S   = n - p;               /* sum of the other two sides */
        idx_t lo  = S > p ? S - p : 1;   /* lower bound for a */
        idx_t hi  = S / 2;               /* upper bound for a */

        if (lo > hi) continue;

        seq_t term1 = (seq_t)hi * (hi + 1);
        term1 /= 2;
        seq_t term2 = (seq_t)(lo - 1) * lo;
        term2 /= 2;

        total += term1 - term2;
    }

    return total;
}
