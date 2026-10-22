/*
 * A372470  Multiplicative with a(p^e) = e+1 if e+1 is a power of 2, and 0
 *          otherwise.
 * https://oeis.org/A372470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e‖n} (e+1) if every e+1 is a power of 2; otherwise
 *            0. The function is multiplicative: for n = ∏ p_i^{e_i}, a(n) = ∏
 *            a(p_i^{e_i}) with a(p^e) = e+1 when e+1 is 2^k, else 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"   /* factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) */

seq_t a(idx_t n)
{
    if (n == 1) return 1;            /* a(1) = 1 */

    if (n == 0) return 0;            /* undefined below offset, safe guard */

    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    seq_t res = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = ex[i];
        uint64_t ek1 = e + 1;               /* e+1 */
        /* check if ek1 is a power of two */
        if ((ek1 & (ek1 - 1)) != 0) {
            return 0;                      /* one exponent fails → whole product is 0 */
        }
        res *= (seq_t)ek1;                  /* ek1 is a power of two, so multiplication fits */
    }
    return res;
}
