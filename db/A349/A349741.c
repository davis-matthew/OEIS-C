/*
 * A349741  a(n) = Product_{k=1..n-1} phi(gcd(n,k)).
 * https://oeis.org/A349741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{d|n, d < n} phi(d)^phi(n/d)
 * Range:     u8 n <= 14, u16 n <= 29, u32 n <= 44, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   big_pow, factor_u64, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 1;

    /* Collect divisors of n (excluding n itself). */
    uint64_t nn = (uint64_t)n;

    uint64_t primes[32];
    uint64_t exps[32];
    size_t prime_cnt = 0;
    factor_u64(nn, primes, exps, &prime_cnt);

    /* Build all divisors.  The maximum number of divisors for n <= 10000
       is comfortably below 256, so a static array suffices. */
    uint64_t divs[256];
    size_t div_cnt = 1;
    divs[0] = 1;

    for (size_t i = 0; i < prime_cnt; ++i)
    {
        size_t cur_len = div_cnt;
        for (size_t j = 0; j < cur_len; ++j)
        {
            uint64_t val = divs[j];
            for (uint64_t e = 1; e <= exps[i]; ++e)
            {
                val *= primes[i];
                divs[div_cnt++] = val;
            }
        }
    }

    /* Compute the product of phi(d)^phi(n/d) for all d < n. */
    seq_t result = 1;
    for (size_t i = 0; i < div_cnt; ++i)
    {
        uint64_t d = divs[i];
        if (d == nn)
            continue;

        seq_t phi_d         = phi((idx_t)d);
        seq_t phi_n_over_d = phi((idx_t)(nn / d));
        seq_t term = big_pow(phi_d, phi_n_over_d);

        result = result * term;
    }

    return result;
}
