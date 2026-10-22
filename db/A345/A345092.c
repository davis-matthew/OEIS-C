/*
 * A345092  a(n) = Sum_{d|n} n^(phi(n/d) - 1).
 * https://oeis.org/A345092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} n^(phi(n/d)-1)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 12, u64 n <= 18, u128 n <= 24,
 *            big any n
 * Helpers:   big_pow, calc_phi, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/calc_phi.h"
#include "../utils/big_pow.h"

/* Generate all positive divisors of n into divs.  divs must
   be large enough to hold them all, and the number of divisors
   is returned through ptr divcnt. */
static void generate_divisors(uint64_t n, uint64_t *divs, size_t *divcnt)
{
    size_t primes_cnt;
    uint64_t primes[10], exps[10];

    factor_u64(n, primes, exps, &primes_cnt);

    /* start with only divisor 1 */
    *divcnt = 1;
    divs[0] = 1;

    /* iterate over prime powers */
    for (size_t i = 0; i < primes_cnt; ++i) {
        size_t cur_cnt = *divcnt;
        uint64_t p = primes[i];
        uint64_t mult = 1;
        for (unsigned e = 1; e <= exps[i]; ++e) {
            mult *= p;
            for (size_t j = 0; j < cur_cnt; ++j) {
                divs[(*divcnt)++] = divs[j] * mult;
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n<1, but safe */

    uint64_t nn = (uint64_t)n;
    uint64_t divs[2000];            /* enough for n <= 10000 (max 128 divisors) */
    size_t divcnt;

    generate_divisors(nn, divs, &divcnt);

    seq_t sum = 0;

    for (size_t i = 0; i < divcnt; ++i) {
        uint64_t d  = divs[i];
        uint64_t m  = nn / d;          /* n/d */
        seq_t phi_m   = calc_phi((seq_t)m);
        seq_t exp     = phi_m - 1;     /* phi(m) - 1, fits in seq_t */
        seq_t term    = big_pow((seq_t)nn, exp);
        sum += term;
    }

    return sum;
}
