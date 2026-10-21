/*
 * A253284  Triangle read by rows, T(n,k) =
 *          (k+1)*(n+1)!*(n+k)!/((k+1)!^2*(n-k)!) with n >= 0 and 0 <= k <= n.
 * https://oeis.org/A253284
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,k), where r = floor((sqrt(8n+1)-1)/2), k = n -
 *            r(r+1)/2, and T(r,k) = (k+1)*(r+1)!*(r+k)!/((k+1)!²*(r-k)!)
 * Range:     u8 n <= 10, u16 n <= 17, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   prime_pow_factorial, prime_pow_number
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/prime_pow_factorial.h"
#include "../utils/prime_pow_number.h"

/* Generate list of primes up to 2000 (sufficient for our limits) */
static uint32_t primes[300];
static size_t primes_cnt = 0;
static void init_primes(void)
{
    if (primes_cnt != 0)
        return;
    bool sieve[2001] = {false};
    for (int i = 2; i <= 2000; ++i)
    {
        if (!sieve[i])
        {
            primes[primes_cnt++] = (uint32_t)i;
            if ((long long)i * i <= 2000)
                for (int j = i * i; j <= 2000; j += i)
                    sieve[j] = true;
        }
    }
}

seq_t a(idx_t n)
{
    /* Determine triangle coordinates (r,k) */
    idx_t r = 0;
    while ((((unsigned __int128)(r + 1) * (r + 2)) / 2) <= n)
        ++r;
    idx_t k = n - r * (r + 1) / 2;

    /* Limit for primes needed: max of (r+1) and (r+k) */
    idx_t limit = r + k;
    if (limit < r + 1)
        limit = r + 1;

    init_primes();
    seq_t result = (seq_t)1;

    for (size_t pi = 0; pi < primes_cnt && primes[pi] <= limit; ++pi)
    {
        uint32_t p = primes[pi];
        int exp = 0;

        /* (r+1)! part */
        exp += prime_pow_factorial(r + 1, p);

        /* product of (r+i) and (r+1-i) for i=1..k */
        for (idx_t i = 1; i <= k; ++i)
        {
            exp += prime_pow_number(r + i, p);
            exp += prime_pow_number(r + 1 - i, p);
            unsigned int ei = prime_pow_number(i, p);
            exp -= 2 * ei;
        }

        /* subtract factor (k+1) */
        exp -= prime_pow_number(k + 1, p);

        /* multiply into result */
        for (int t = 0; t < exp; ++t)
            result *= (seq_t)p;
    }

    return result;
}
