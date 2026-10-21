/*
 * A293514  a(n) = Product_{d|n, d>1} prime(A286561(n,d)), where A286561(n,d)
 *          gives the highest exponent of d dividing n.
 * https://oeis.org/A293514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{d|n, d>1} prime( A286561(n,d) )
 * Range:     u8 n <= 23, u16 n <= 119, u32 n <= 719, u64 n <= 7559,
 *            u128 n <= 9909, big any n
 * Helpers:   factor_u64, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where A286561(n,d) is the largest e such that d^e divides n */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"   /* factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */
#include "../utils/get_prime.h"   /* get_prime(idx_t k) -> uint32_t */

static void _rec_divisors(size_t pos,
                          const uint64_t *pr,
                          const uint64_t *exp,
                          size_t m,
                          uint64_t *sel,
                          seq_t *prod)
{
    if (pos == m) {
        /* skip divisor 1 */
        int allzero = 1;
        for (size_t k = 0; k < m; ++k) {
            if (sel[k]) { allzero = 0; break; }
        }
        if (allzero) return;

        /* compute valuation of this divisor in n */
        uint64_t e = UINT64_MAX;
        for (size_t k = 0; k < m; ++k) {
            if (sel[k]) {
                uint64_t cand = exp[k] / sel[k];
                if (cand < e) e = cand;
            }
        }

        /* multiply the corresponding prime */
        seq_t p = (seq_t)get_prime(e);
        *prod *= p;
        return;
    }

    for (uint64_t f = 0; f <= exp[pos]; ++f) {
        sel[pos] = f;
        _rec_divisors(pos + 1, pr, exp, m, sel, prod);
    }
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;

    /* maximum number of distinct prime factors for 64‑bit numbers is 16 */
    uint64_t pr[16];
    uint64_t exp[16];
    size_t  m = 0;

    factor_u64(n, pr, exp, &m);

    uint64_t sel[16] = {0};
    seq_t prod = (seq_t)1;

    _rec_divisors(0, pr, exp, m, sel, &prod);

    return prod;
}
