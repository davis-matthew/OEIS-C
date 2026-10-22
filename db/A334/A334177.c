/*
 * A334177  Squarefree part of numerator of the squared area of the Heronian
 *          triangle with sequential prime sides whose shortest leg is
 *          prime(n).
 * https://oeis.org/A334177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   factor_u64, get_prime, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  Formula: a(n) = squarefree part of the numerator of the squared area
 *             of the Heronian triangle with sequential prime sides
 *             (p_n, p_{n+1}, p_{n+2}), where p_k is the k-th prime.
 *             For each n≥1, let
 *                 s = (p_n + p_{n+1} + p_{n+2}) / 2
 *                 A² = s*(s-p_n)*(s-p_{n+1})*(s-p_{n+2})
 *             then a(n) = squarefree_part( numerator of A² after reduction ).
 *  This implementation factors all terms individually and accumulates
 *  the parity of each prime factor to obtain the squarefree part.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/get_prime.h"
#include "../utils/factor_u64.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    if (n == 1)       /* degenerate triangle {2,3,5} */
        return 0;

    /* primes of the triangle */
    uint64_t p = get_prime(n);
    uint64_t q = get_prime(n + 1);
    uint64_t r = get_prime(n + 2);

    /* the four factors in the numerator of A² = product / 16 */
    uint64_t term[4];
    term[0] = p + q + r;          /* p+q+r */
    term[1] = q + r - p;          /* q+r-p   */
    term[2] = p + r - q;          /* p+r-q   */
    term[3] = p + q - r;          /* p+q-r   */

    /* acceleration: if any term is 0, area is 0 */
    if (term[3] == 0)
        return 0;

    /* parity table for prime factors up to 400000 (safe limit) */
    static bool parity[400001] = { 0 };
    /* list of primes that were toggled during the current call */
    static uint32_t used[256];
    size_t used_cnt = 0;

    for (int i = 0; i < 4; ++i) {
        uint64_t v = term[i];
        if (v <= 1)          /* skip 0 or 1 */
            continue;

        /* factor v into prime powers */
        uint64_t pr[20];
        uint64_t ex[20];
        size_t cnt;
        factor_u64(v, pr, ex, &cnt);

        for (size_t j = 0; j < cnt; ++j) {
            if (ex[j] & 1U) {                /* odd exponent -> toggle */
                uint32_t prime = (uint32_t)pr[j];
                if (!parity[prime]) {
                    parity[prime] = true;
                    used[used_cnt++] = prime;
                } else {
                    parity[prime] = false;
                }
            }
        }
    }

    /* calculate the product of primes with odd parity */
    seq_t res = 1;
    for (size_t i = 0; i < used_cnt; ++i) {
        if (parity[used[i]])                /* still odd after all toggles */
            res = mul_small(res, used[i]);

        parity[used[i]] = false;            /* reset for next call */
    }

    return res;
}
