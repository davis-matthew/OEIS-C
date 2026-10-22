/*
 * A342612  a(n) = Sum_{d|n} phi(n/d)^(n-d).
 * https://oeis.org/A342612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} phi(n/d)^(n-d)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   big_pow, calc_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/big_pow.h"
#include "../utils/calc_phi.h"

/* Helper: generate all divisors of n from its prime factorisation
   and accumulate the summation */
static void
rec_divisors(size_t idx, idx_t cur, idx_t n,
             const idx_t *primes, const int *exps, size_t pcnt,
             seq_t *sum)
{
    if (idx == pcnt) {
        idx_t d = cur;
        idx_t nd = n - d;
        seq_t phi_val = calc_phi((seq_t)(n / d));
        seq_t term = big_pow(phi_val, (seq_t)nd);
        *sum += term;
        return;
    }
    idx_t p = primes[idx];
    idx_t mult = 1;
    for (int e = 0; e <= exps[idx]; ++e) {
        rec_divisors(idx + 1, cur * mult, n, primes, exps, pcnt, sum);
        mult *= p;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, but harmless */

    /* Factorise n into primes and exponents */
    idx_t primes[32];
    int exps[32];
    size_t pcnt = 0;

    idx_t m = n;
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            int exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            primes[pcnt] = p;
            exps[pcnt] = exp;
            ++pcnt;
        }
    }
    if (m > 1) {
        primes[pcnt] = m;
        exps[pcnt] = 1;
        ++pcnt;
    }

    seq_t total = 0;
    rec_divisors(0, 1, n, primes, exps, pcnt, &total);
    return total;
}
