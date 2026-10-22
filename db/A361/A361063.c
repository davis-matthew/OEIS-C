/*
 * A361063  Multiplicative with a(p^e) = sigma_2(e), where sigma_2 = A001157.
 * https://oeis.org/A361063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e‖n} σ₂(e) , σ₂(e)=∑_{d∣e} d²
 * Range:     u8 n <= 863, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, sigma2_of_e
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/sigma2_of_e.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;   /* a(1) = 1 */

    uint64_t primes[20], exps[20];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t res = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t s = sigma2_of_e(exps[i]);
        res *= s;
    }
    return res;
}
