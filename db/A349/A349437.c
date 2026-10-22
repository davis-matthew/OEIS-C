/*
 * A349437  Dirichlet convolution of A252463 with A055615 (Dirichlet inverse
 *          of n), where A252463 shifts the prime factorization of odd numbers
 *          one step towards smaller primes and divides even numbers by two.
 * https://oeis.org/A349437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = ∑_{d|n} A055615(n/d) * A252463(d)
 * Range:     i8 n <= 840, i16 n <= 9951, i32 n <= 9951, i64 n <= 9951,
 *            i128 n <= 9951, big any n
 * Helpers:   factor_u64, prev_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A055615: Dirichlet inverse of the sequence f(n)=n.
   A252463: For even d → d/2; for odd d → shift each odd prime factor to its predecessor prime. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/prev_prime.h"

static void rec(size_t pos, uint64_t d_val,
                uint64_t cur_exp[],
                const uint64_t primes[],
                const uint64_t exps[],
                size_t num,
                uint64_t orig_n,
                seq_t *sum)
{
    if (pos == num) {
        /* Compute A252463(d_val) */
        seq_t A252;
        if (d_val == 1) {
            A252 = 1;
        } else if ((d_val & 1ULL) == 0) {
            A252 = (seq_t)(d_val >> 1);
        } else {
            uint64_t prod = 1;
            for (size_t i = 0; i < num; ++i) {
                uint64_t e = cur_exp[i];
                if (e == 0) continue;
                /* p == 2 will never appear for odd d_val */
                uint64_t base = prev_prime(primes[i]);
                for (uint64_t j = 0; j < e; ++j)
                    prod *= base;
            }
            A252 = (seq_t)prod;
        }

        /* Compute A055615(n/d_val)  i.e. Dirichlet inverse of n evaluated at m = orig_n / d_val */
        uint64_t m_exp;
        bool squarefree = true;
        seq_t prod = 1;
        int parity = 0; /* number of primes in m (mod 2) */
        for (size_t i = 0; i < num; ++i) {
            m_exp = exps[i] - cur_exp[i];
            if (m_exp > 1) {
                squarefree = false;
                break;
            } else if (m_exp == 1) {
                parity ^= 1;
                prod *= primes[i];
            }
        }
        seq_t g_m;
        if (!squarefree) {
            g_m = 0;
        } else {
            g_m = (parity == 0) ? prod : -prod;
        }

        *sum += g_m * A252;
        return;
    }

    uint64_t d = d_val;
    for (uint64_t e = 0; e <= exps[pos]; ++e) {
        cur_exp[pos] = e;
        rec(pos + 1, d, cur_exp, primes, exps, num, orig_n, sum);
        d *= primes[pos];
    }
}

seq_t a(idx_t n)
{
    uint64_t primes[64];
    uint64_t exps[64];
    size_t num = 0;

    if (n <= 1) return 1; /* a(1) = 1, and n>=1 per spec */

    factor_u64((uint64_t)n, primes, exps, &num);

    uint64_t cur_exp[64] = {0};
    seq_t sum = 0;

    rec(0, 1, cur_exp, primes, exps, num, (uint64_t)n, &sum);

    return sum;
}
