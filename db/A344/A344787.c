/*
 * A344787  a(n) = n * Sum_{d|n} sigma_d(d) / d, where sigma_k(n) is the sum
 *          of the k-th powers of the divisors of n.
 * https://oeis.org/A344787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * Sum_{d|n} sigma_d(d) / d where sigma_k(m) = Sum_{e|m}
 *            e^k
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dependencies:
   - factor_u64: factorisation of an integer
   - big_pow: arbitrary‑precision exponentiation
   Only standard headers are included; seq_t and idx_t are globals from the build.
*/

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

/* Recursively generate all divisors of a number given its prime factors.
   pf   – array of distinct prime factors
   pe   – corresponding exponents
   pos  – current index into pf/pe
   num  – total number of distinct primes
   cur  – current divisor being built
   arr  – output array
   cnt  – current count of divisors written to arr
*/
static void
gen_divs_rec(const uint64_t *pf, const uint64_t *pe, int pos, int num,
             idx_t cur, idx_t *arr, size_t *cnt)
{
    if (pos == num) {
        arr[*cnt] = cur;
        ++(*cnt);
        return;
    }
    uint64_t p = pf[pos];
    uint64_t e = pe[pos];
    idx_t tmp = cur;
    for (uint64_t i = 0; i <= e; ++i) {
        gen_divs_rec(pf, pe, pos + 1, num, tmp, arr, cnt);
        tmp *= p;
    }
}

/* Main function computing the sequence value. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* n>=1 by problem statement */

    /* ---------- 1. Factorise n once ---------- */
    uint64_t primes[32];
    uint64_t exps_n[32];
    size_t pn = 0;
    factor_u64(n, primes, exps_n, &pn);

    /* ---------- 2. List all divisors of n ---------- */
    idx_t divs_n[256];
    size_t nd_n = 0;
    gen_divs_rec(primes, exps_n, 0, (int)pn, 1, divs_n, &nd_n);

    /* ---------- 3. Compute the sum over all divisors ---------- */
    seq_t total = 0;

    for (size_t idx = 0; idx < nd_n; ++idx) {
        idx_t d = divs_n[idx];

        /* ---- Compute sigma_d(d) by summing k^d over all k|d ---- */
        /* Determine exponents of each prime in d */
        uint64_t exps_d[32];
        for (size_t j = 0; j < pn; ++j) {
            uint64_t cnt = 0;
            idx_t tmp = d;
            while (tmp % primes[j] == 0) {
                ++cnt;
                tmp /= primes[j];
            }
            exps_d[j] = cnt;
        }

        /* Generate all divisors of d (using the same prime list) */
        idx_t divs_d[256];
        size_t nd_d = 0;
        gen_divs_rec(primes, exps_d, 0, (int)pn, 1, divs_d, &nd_d);

        /* Sum k^d for each divisor k of d */
        seq_t sigma = 0;
        for (size_t k = 0; k < nd_d; ++k) {
            idx_t kdiv = divs_d[k];
            sigma += big_pow((seq_t)kdiv, (seq_t)d);
        }

        /* term = (n/d) * sigma_d(d) */
        seq_t term = (seq_t)(n / d) * sigma;
        total += term;
    }

    return total;
}
