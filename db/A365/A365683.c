/*
 * A365683  The largest exponentially squarefree divisor of n.
 * https://oeis.org/A365683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e‖n} p^{A070321(e)} where * A070321(e) is the
 *            largest square‑free integer ≤ e
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Pre‑compute A070321(e) for e = 0…64 (enough for 64‑bit n). */
    static unsigned char f[65];
    static bool initialized = false;
    if (!initialized) {
        bool is_sqfree[65];
        is_sqfree[0] = false;     /* unused */
        is_sqfree[1] = true;
        for (int i = 2; i <= 64; ++i) {
            bool sf = true;
            for (int j = 2; j * j <= i; ++j) {
                if (i % (j * j) == 0) { sf = false; break; }
            }
            is_sqfree[i] = sf;
        }
        f[0] = 0;
        for (int i = 1; i <= 64; ++i)
            f[i] = is_sqfree[i] ? (unsigned char)i : f[i-1];
        initialized = true;
    }

    /* Factor n into primes and exponents. */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        unsigned char sf_exp = f[(int)exps[i]];
        seq_t part = pow_uint(primes[i], (idx_t)sf_exp);
        result *= part;
    }
    return result;
}
