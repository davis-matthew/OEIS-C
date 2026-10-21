/*
 * A242250  Least positive integer g < prime(n) such that g, 2^g - 1 and
 *          (g-1)! are all primitive roots modulo prime(n), or 0 if such a
 *          number g does not exist.
 * https://oeis.org/A242250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least positive integer g < prime(n) such that g, 2^g-1
 *            and (g-1)! are all primitive roots modulo prime(n), or 0 if no
 *            such g exists. The function computes this by iterating g from 1
 *            up to prime(n)-1, testing the primitive‑root condition for the
 *            three values. Primitive root test uses the fa...
 * Range:     u8 n <= 54, u16 n <= 7218, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/mod_pow.h"

static bool is_primitive_root(seq_t val, uint32_t p, uint32_t m,
                              const uint32_t *pf, size_t pf_cnt) {
    /* val must be reduced modulo p beforehand */
    if (val % (seq_t)p == 0) return false;
    for (size_t i = 0; i < pf_cnt; ++i) {
        uint32_t q = pf[i];
        if (mod_pow(val, (uint64_t)(m / q), p) == 1) return false;
    }
    return true;
}

seq_t a(idx_t n) {
    /* n is 1‑based index */
    uint32_t p = get_prime((uint64_t)n);          /* prime(n) */
    if (p < 2) return 0;                          /* safety */

    uint32_t m = p - 1;                           /* p-1 */

    /* distinct prime factors of m */
    uint32_t pf[64];
    size_t pf_cnt = 0;
    uint32_t temp = m;
    for (uint32_t d = 2; (uint64_t)d * d <= temp; ++d) {
        if (temp % d == 0) {
            pf[pf_cnt++] = d;
            while (temp % d == 0) temp /= d;
        }
    }
    if (temp > 1) pf[pf_cnt++] = temp;

    seq_t fact = 1;                               /* (g-1)! modulo p */
    seq_t r    = 2 % p;                            /* 2^1 mod p */

    for (uint32_t g = 1; g < p; ++g) {
        /* test g itself */
        if (!is_primitive_root((seq_t)g, p, m, pf, pf_cnt))
            /* skip update to maintain correct fact for next g */
            ;
        else {
            /* test 2^g - 1 */
            seq_t r_minus = (r == 0 ? (seq_t)(p - 1) : r - 1);
            if (!is_primitive_root(r_minus, p, m, pf, pf_cnt))
                ;
            else {
                /* test (g-1)! */
                if (is_primitive_root(fact, p, m, pf, pf_cnt))
                    return (seq_t)g;
            }
        }

        /* prepare for next g */
        fact = (fact * (seq_t)g) % p;             /* g! mod p */
        r    = (r * 2) % p;                       /* 2^(g+1) mod p */
    }

    return 0;                                     /* no g found */
}
