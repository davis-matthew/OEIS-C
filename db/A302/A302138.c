/*
 * A302138  Period of Kronecker symbol modulo n.
 * https://oeis.org/A302138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   Multiplicative with a(1)=1 and a(p^e) = p (p odd), a(2^e) = 8 (e
 *            odd), 2 (e even). Then a(n) is the lcm of the a(p^e) for all
 *            prime powers dividing n
 * Range:     u8 n <= 65, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    if (n == 1u) return (seq_t)1;
    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &cnt);
    seq_t period = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t term;
        if (pr[i] == 2ULL) {
            term = (ex[i] & 1ULL) ? (seq_t)8 : (seq_t)2;
        } else {
            term = (seq_t)pr[i];
        }
        seq_t g = gcd(period, term);
        period = (period / g) * term;
    }
    return period;
}
