/*
 * A345046  If n = Product p(k)^e(k) then a(n) = LCM (p(k)-1)^e(k).
 * https://oeis.org/A345046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm{(p-1)^e : n = Π p^e} (n≥1, a(1)=1)
 * Range:     u8 n <= 256, u16 n <= 9139, u32 n <= 9139, u64 n <= 9139,
 *            u128 n <= 9139, big any n
 * Helpers:   factor_u64, seq_gcd, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_gcd.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n) {
    if (n <= 1) return 1;                  /* a(1) = 1 */
    uint64_t primes[64], exps[64];
    size_t cnt;
    factor_u64(n, primes, exps, &cnt);     /* factor n into primes and exponents */
    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = primes[i];
        seq_t e = exps[i];
        seq_t base = p - 1;
        seq_t term = seq_pow(base, (idx_t)e);           /* (p-1)^e */
        seq_t g = seq_gcd(result, term);
        seq_t tmp = result / g;                        /* result/g * term */
        result = seq_mul(tmp, term);
    }
    return result;
}
