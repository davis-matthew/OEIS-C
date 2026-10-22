/*
 * A371306  Least positive number k such that binomial(k^2,k) is divisible by
 *          n.
 * https://oeis.org/A371306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2186, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A371306
 *
 *  a(n) = least positive integer k such that binomial(k^2, k) ≡ 0  (mod n)
 *
 *  The implementation factorises n, then scans k ≥ 1 until all prime‑power
 *  divisibility conditions are satisfied.  For each prime p dividing n we
 *  evaluate the p‑adic valuation of the binomial coefficient using the
 *  Legendre formula:
 *
 *      v_p(C(m,r)) = v_p(m!) – v_p(r!) – v_p((m−r)!)
 *
 *  For our problem m = k^2 and r = k.  The loop is fast for the required
 *  range (n ≤ 10000), and the function is re‑entrant and does not depend
 *  on call order.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"          /* factorization utility */

/*  v_p(m!)  using Legendre's formula  */
static uint32_t vp_factorial(uint64_t m, uint64_t p)
{
    uint32_t v = 0;
    while (m) {
        m /= p;
        v += (uint32_t)m;
    }
    return v;
}

seq_t a(idx_t n)
{
    if (n == 1)                                      /* a(1) = 1 */
        return (seq_t)1;

    /* ---------- factorise n -------------------------------------------- */
    uint64_t primes[32], exps[32];
    size_t  pcnt = 0;
    factor_u64((uint64_t)n, primes, exps, &pcnt);

    /* ---------- search for smallest k ------------------------------- */
    for (idx_t k = 1;; ++k) {
        bool ok = true;
        uint64_t kk = (uint64_t)k * (uint64_t)k;           /* k^2      */
        for (size_t i = 0; i < pcnt; ++i) {
            uint64_t p = primes[i];
            uint64_t e = exps[i];
            uint32_t vp = vp_factorial(kk, p)
                        - vp_factorial((uint64_t)k, p)
                        - vp_factorial(kk - (uint64_t)k, p);
            if (vp < e) {
                ok = false;
                break;
            }
        }
        if (ok)
            return (seq_t)k;      /* found least k */
    }
}
