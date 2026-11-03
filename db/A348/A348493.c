/*
 * A348493  a(n) = A003415(n) / gcd(A003415(n), A018804(n)), where A003415 is
 *          the arithmetic derivative and A018804 is Pillai's arithmetical
 *          function.
 * https://oeis.org/A348493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003415(n) / gcd(A003415(n), A018804(n))
 * Range:     u8 n <= 62, u16 n <= 7249, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 111 terms through n = 9735 (2026-11-01)
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"

static void sum_pillai_recursive(const uint64_t *pr, const uint64_t *ex, size_t cnt, size_t idx,
                                 uint64_t cur_d, uint64_t *farr, seq_t *pil)
{
    if (idx == cnt) {
        seq_t phi_val = 1;
        for (size_t i = 0; i < cnt; ++i) {
            uint64_t e_rem = ex[i] - farr[i];
            if (e_rem == 0)
                continue;
            seq_t p_pow_minus1 = 1;
            for (uint64_t t = 0; t < e_rem - 1; ++t)
                p_pow_minus1 *= (seq_t)pr[i];
            seq_t phi_part = p_pow_minus1 * ((seq_t)pr[i] - 1);
            phi_val *= phi_part;
        }
        pil[0] += ((seq_t)cur_d) * phi_val;
        return;
    }

    for (uint64_t e = 0; e <= ex[idx]; ++e) {
        farr[idx] = e;
        uint64_t new_d = cur_d;
        for (uint64_t t = 0; t < e; ++t)
            new_d *= pr[idx];
        sum_pillai_recursive(pr, ex, cnt, idx + 1, new_d, farr, pil);
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* out of offset */

    uint64_t n64 = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    factor_u64(n64, primes, exps, &cnt);

    /* Arithmetic derivative A003415 */
    seq_t der = 0;
    seq_t nseq = (seq_t)n;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t term = nseq / (seq_t)p;
        term *= (seq_t)e;
        der += term;
    }

    /* Pillai’s arithmetical function A018804 */
    seq_t pil = 0;
    if (cnt == 0) {         /* n == 1 */
        pil = 1;
    } else {
        uint64_t *farr = (uint64_t *)calloc(cnt, sizeof(uint64_t));
        sum_pillai_recursive(primes, exps, cnt, 0, 1, farr, &pil);
        free(farr);
    }

    seq_t g = gcd(der, pil);
    return der / g;
}
