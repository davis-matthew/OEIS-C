/*
 * A294875  a(n) = Product_{d|n, d = x^k, with x,k > 1} prime(A052409(d)-1).
 * https://oeis.org/A294875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{d|n, d = x^k, k>1} prime(k−1) where k is the maximal
 *            exponent such that d = x^k. The prime function returns the
 *            (k−1)-th prime (k≥2)
 * Range:     u8 n <= 63, u16 n <= 255, u32 n <= 2047, u64 n <= 9836,
 *            u128 n <= 9836, big any n
 * Helpers:   factor_u64, gcd_int, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd_int.h"
#include "../utils/get_prime.h"

/* Recursive helper that enumerates all divisors of n, keeping track of the
   greatest common divisor of the exponents chosen for the current divisor.
   If the divisor is >1 and the gcd of its exponents is ≥2, it contributes
   a factor prime(gcd-1) to the result. */
static void dfs(size_t pos,
                idx_t prod,
                int g,
                uint64_t *pr,
                uint32_t *ex,
                size_t m,
                seq_t *res)
{
    if (pos == m) {
        if (prod > 1 && g >= 2) {
            /* prime(g-1) where g is the maximal exponent of this divisor */
            uint32_t p = get_prime(g - 1);
            *res *= p;
        }
        return;
    }

    idx_t cur = prod;
    for (uint32_t e = 0; e <= ex[pos]; ++e) {
        int ng = (e == 0) ? g : (g == 0 ? (int)e : gcd_int(g, (int)e));
        dfs(pos + 1, cur, ng, pr, ex, m, res);
        if (e == ex[pos]) break;
        cur *= pr[pos];
    }
}

seq_t a(idx_t n)
{
    if (n <= 1) return 1;

    /* Factorization of n.  Use a stack‑allocated buffer for small n. */
    uint64_t pf[64];
    uint64_t ef[64];
    size_t cnt = 0;
    factor_u64(n, pf, ef, &cnt);

    /* Copy exponents to a 32‑bit array for gcd_int. */
    uint32_t ex[64];
    for (size_t i = 0; i < cnt; ++i) ex[i] = (uint32_t)ef[i];

    seq_t result = 1;
    dfs(0, 1, 0, pf, ex, cnt, &result);
    return result;
}
