/*
 * A334489  a(n) = Product_{d|n} (pod(n)/pod(d)) where pod(n) = A007955(n),
 *          the product of divisors of n.
 * https://oeis.org/A334489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{d|n} (pod(n)/pod(d)) where pod(m) = product of
 *            divisors of m. The implementation factors n, computes the
 *            exponent of each prime in a(n) via the closed‑form exp = e_i *
 *            (3·tau² – 2·M) / 6, and returns the product
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 11, u64 n <= 23, u128 n <= 29,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    /* factor n */
    uint64_t x = n;
    size_t cnt = 0;
    uint64_t primes[16];
    uint64_t exps[16];
    for (uint64_t p = 2; p * p <= x; ++p)
    {
        if (x % p == 0)
        {
            uint64_t e = 0;
            while (x % p == 0) { x /= p; ++e; }
            primes[cnt] = p;
            exps[cnt] = e;
            ++cnt;
        }
    }
    if (x > 1)
    {
        primes[cnt] = x;
        exps[cnt] = 1;
        ++cnt;
    }

    /* tau(n) = product (e_i + 1) */
    uint64_t tau = 1;
    for (size_t i = 0; i < cnt; ++i) tau *= (exps[i] + 1);

    /* M = product T_i,   T_i = (e_i+1)(e_i+2)/2 */
    unsigned __int128 M = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        unsigned __int128 Ti = ((unsigned __int128)(exps[i] + 1) *
                                (exps[i] + 2)) / 2;
        M *= Ti;
    }

    unsigned __int128 tau2     = (unsigned __int128)tau * tau;
    unsigned __int128 numerator = 3 * tau2 - 2 * M;

    /* compute a(n) = Π p_i^exp_i */
    seq_t ans = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i)
    {
        unsigned __int128 exp128 = (unsigned __int128)exps[i] * numerator / 6;
        seq_t term = seq_pow((seq_t)primes[i], (idx_t)exp128);
        ans = seq_mul(ans, term);
    }
    return ans;
}
