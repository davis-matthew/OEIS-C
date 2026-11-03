/*
 * A361063  Multiplicative with a(p^e) = sigma_2(e), where sigma_2 = A001157.
 * https://oeis.org/A361063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e‖n} σ₂(e) , σ₂(e)=∑_{d∣e} d²
 * Range:     u8 n <= 863, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 100 terms through n = 9911 (2026-11-01)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

/* Compute σ₂(e)=∑_{d∣e} d²  (e is at most 64 for n≤2⁶⁴) */
static seq_t sigma2_of_e(uint64_t e)
{
    seq_t sum = 0;
    for (uint64_t d = 1; d * d <= e; ++d) {
        if (e % d == 0) {
            seq_t d2 = (seq_t)d * (seq_t)d;
            sum += d2;
            uint64_t o = e / d;
            if (o != d) {
                seq_t o2 = (seq_t)o * (seq_t)o;
                sum += o2;
            }
        }
    }
    return sum;
}

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
