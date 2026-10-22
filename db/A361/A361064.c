/*
 * A361064  Multiplicative with a(p^e) = sigma_3(e), where sigma_3 = A001158.
 * https://oeis.org/A361064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e‖n} sigma_3(e) (sigma_3 = Σ_{d|e} d^3)
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/sigma_k.h"

seq_t a(idx_t n) {
    if (n == 1) {
        return 1;
    }
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64(n, primes, exps, &cnt);
    seq_t res = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t v = sigma_k(exps[i], 3);
        res *= v;
    }
    return res;
}
