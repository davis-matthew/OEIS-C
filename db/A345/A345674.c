/*
 * A345674  Euler totient function phi(n) - number of primitive roots modulo
 *          n.
 * https://oeis.org/A345674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = φ(n) - (φ(φ(n)) if n has primitive roots else 0)
 *            Primitive roots exist for n = 1, 2, 4, p^k (p odd prime), or
 *            2·p^k (p odd prime)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   calc_phi, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/calc_phi.h"

seq_t a(idx_t n)
{
    /* φ(n) and check existence of primitive roots */
    uint64_t n64 = (uint64_t)n;

    /* compute φ(n) using the provided utility */
    seq_t phi_n = calc_phi((seq_t)n64);

    /* factor n to decide if a primitive root exists */
    uint64_t primes[64], exponents[64];
    size_t pf_cnt = 0;
    factor_u64(n64, primes, exponents, &pf_cnt);

    bool has_pr = false;

    /* trivial cases */
    if (n64 == 1 || n64 == 2 || n64 == 4) {
        has_pr = true;
    } else {
        /* odd prime power p^k, p odd */
        if (pf_cnt == 1 && primes[0] != 2) {
            has_pr = true;
        } else if ((n64 & 1) == 0) {  /* even n */
            /* look for factor 2 with exponent 1 */
            size_t idx2 = pf_cnt;
            for (size_t i = 0; i < pf_cnt; ++i) {
                if (primes[i] == 2) {
                    idx2 = i;
                    break;
                }
            }
            if (idx2 < pf_cnt && exponents[idx2] == 1) {
                /* remaining part must be a single odd prime power */
                if (pf_cnt == 2) {
                    /* the other prime is necessarily odd */
                    has_pr = true;
                }
            }
        }
    }

    if (!has_pr) {
        return phi_n;  /* no primitive roots */
    }

    /* φ(φ(n)) */
    seq_t phi_phi = calc_phi(phi_n);

    /* a(n) = φ(n) – φ(φ(n)) */
    return phi_n - phi_phi;
}
