/*
 * A396114  a(n) is the first multiple of n encountered when reading Pascal's
 *          triangle row-by-row, left to right.
 * https://oeis.org/A396114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 45, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search rows r = 0…n and columns k = 0…r in order,
   returning the first binomial coefficient C(r,k) that is
   divisible by n.  The algorithm factorises n,
   pre‑computes the valuation of each prime divisor in all
   factorials up to n, and then checks the divisibility
   condition for each pair (r,k).  This guarantees the
   minimal (r,k) in the row‑by‑row order. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/binomial.h"   /* seq_t binomial(idx_t n, idx_t k); */

seq_t a(idx_t n)
{
    /* factorise n by trial division (n ≤ 10000 in the test range) */
    uint64_t primes[32];
    unsigned int exps[32];
    size_t pc = 0;
    uint64_t tmp = (uint64_t)n;
    for (idx_t p = 2; (uint64_t)p * (uint64_t)p <= tmp; ++p) {
        if (tmp % p == 0) {
            unsigned int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes[pc]   = (uint64_t)p;
            exps[pc]     = e;
            ++pc;
        }
    }
    if (tmp > 1) {
        primes[pc]   = tmp;
        exps[pc]     = 1;
        ++pc;
    }

    /* pre‑compute v_p(j!) for all j ≤ n and all p | n */
    size_t stride = (size_t)n + 1;            /* include 0! */
    uint32_t *expf = (uint32_t *)malloc(pc * stride * sizeof(uint32_t));
    if (expf == NULL)
        return binomial(n, (idx_t)1);         /* fallback, should not happen */

    for (size_t j = 0; j < pc; ++j)           /* expf[j*stride + 0] = 0 */
        expf[j * stride] = 0;

    for (size_t i = 1; i <= (size_t)n; ++i) {
        for (size_t j = 0; j < pc; ++j) {
            uint64_t p = primes[j];
            uint32_t e = 0;
            uint64_t m = i;
            while (m % p == 0) {
                ++e;
                m /= p;
            }
            expf[j * stride + i] = expf[j * stride + i - 1] + e;
        }
    }

    /* scan rows and columns in the required order */
    for (idx_t r = 0; r <= n; ++r) {
        size_t rn = (size_t)r;
        for (idx_t k = 0; k <= r; ++k) {
            size_t kn = (size_t)k;
            bool ok = true;
            for (size_t j = 0; j < pc; ++j) {
                uint32_t need = exps[j];
                uint32_t exp =
                    expf[j * stride + rn] - expf[j * stride + kn]
                    - expf[j * stride + (rn - kn)];
                if (exp < need) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                seq_t val = binomial(r, k);
                free(expf);
                return val;
            }
        }
    }

    /* Should never reach here: C(n,1)=n is always a candidate */
    seq_t val = binomial(n, (idx_t)1);
    free(expf);
    return val;
}
