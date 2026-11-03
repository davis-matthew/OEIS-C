/*
 * A333387  Number of solutions to x^y == 1 (mod n) where 0 <= x < n and 1 <=
 *          y <= n.
 * https://oeis.org/A333387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{1≤k≤n, gcd(n,k)=1} ⌊n ÷ order(k,n)⌋ where order(k,n)
 *            is the multiplicative order of k modulo n
 * Range:     u8 n <= 39, u16 n <= 1196, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9447 (2026-11-01)
 * Helpers:   factor_u64, gcd, powmod, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"
#include "../utils/powmod.h"
#include "../utils/totient.h"

seq_t a(idx_t n) {
    if (n == 1) {
        return (seq_t)1;
    }

    seq_t n_seq = (seq_t)n;
    seq_t phi_n = totient(n);          /* φ(n) */

    /* Factor φ(n).  φ(n) ≤ n ≤ 10000, so 32 prime factors are enough. */
    uint64_t fac_pr[32];
    uint64_t fac_exp[32];
    size_t fac_cnt = 0;
    factor_u64((uint64_t)phi_n, fac_pr, fac_exp, &fac_cnt);

    seq_t result = 0;

    /* Iterate over all units modulo n. */
    for (idx_t k = 1; k < n; ++k) {
        if (gcd((seq_t)k, n_seq) != (seq_t)1) {
            continue;            /* not a unit */
        }

        seq_t order = phi_n;      /* current candidate order */

        /* Reduce order using the prime factorization of φ(n). */
        for (size_t fi = 0; fi < fac_cnt; ++fi) {
            uint64_t p   = fac_pr[fi];
            uint64_t exp = fac_exp[fi];

            for (uint64_t e = 0; e < exp; ++e) {
                if (order % (seq_t)p != (seq_t)0) {
                    break;
                }
                seq_t cand = order / (seq_t)p;
                if (powmod((seq_t)k, (uint64_t)cand, n_seq) == (seq_t)1) {
                    order = cand;
                } else {
                    break;
                }
            }
        }

        result += n_seq / order;
    }

    return result;
}
