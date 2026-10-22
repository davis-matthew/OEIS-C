/*
 * A329586  Row lengths of A329585: number of solutions of the congruences x^2
 *          == +1 (mod n) or (inclusive) x^2 == -1 (mod n), for n >= 1.
 * https://oeis.org/A329586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 (n=1), 2 (n=2), and for n≥3: a(n) = 2^{r2(e2)+r1+r3} +
 *            δ_{e2≤1}·δ_{r3=0}·2^{r1}, where e2 is the exponent of 2 in n, r1
 *            is the number of distinct odd primes ≡1 mod 4 dividing n, r3 is
 *            the number of distinct odd primes ≡3 mod 4 dividing n
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   factor_u64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    size_t r1 = 0, r3 = 0;
    idx_t e2 = 0;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        if (p == 2) {
            e2 = (idx_t)e;
            continue;
        }
        if (p % 4 == 1) ++r1;
        else if (p % 4 == 3) ++r3;
    }

    int r2 = 0;
    if (e2 >= 2) r2 = 1;
    if (e2 >= 3) r2 = 2;

    seq_t result = pow2((idx_t)(r2 + r1 + r3));
    if (e2 <= 1 && r3 == 0) {
        result += pow2((idx_t)r1);
    }

    return result;
}
