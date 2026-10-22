/*
 * A344773  Number of divisors d of n for which A342915(d) = A342915(n), where
 *          A342915(n) = gcd(1+n, psi(n)).
 * https://oeis.org/A344773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} [ gcd(1+d, ψ(d)) == gcd(1+n, ψ(n)) ]
 * Range:     u8 n <= 865, u16 n <= 9472, u32 n <= 9472, u64 n <= 9472,
 *            u128 n <= 9472, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where ψ(n) = n * Prod_{p|n}(1+1/p) = Prod_{p^e||n} (p^e + p^(e-1)) */
#include <stddef.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"

static void count_divisors(idx_t idx,
                           idx_t cur_d,
                           seq_t cur_psi,
                           const uint64_t *primes,
                           const uint64_t *exps,
                           size_t cnt,
                           seq_t target_g,
                           idx_t *res)
{
    if (idx == cnt) {
        seq_t g = gcd((seq_t)cur_d + 1, cur_psi);
        if (g == target_g) (*res)++;
        return;
    }

    uint64_t p   = primes[idx];
    size_t   e   = (size_t)exps[idx];

    idx_t d_mult  = 1;   /* incrementally multiply the prime power */
    seq_t psi_mult = 1;  /* corresponding ψ multiplier for this prime */

    /* Case k = 0: prime not in divisor */
    count_divisors(idx + 1, cur_d, cur_psi, primes, exps, cnt, target_g, res);

    for (size_t k = 1; k <= e; ++k) {
        d_mult *= p;
        if (k == 1) {
            psi_mult = (seq_t)(p + 1);          /* p^1 + p^0 */
        } else {
            psi_mult *= p;                      /* multiply by p for higher powers */
        }
        count_divisors(idx + 1,
                       cur_d * d_mult,
                       cur_psi * psi_mult,
                       primes,
                       exps,
                       cnt,
                       target_g,
                       res);
    }
}

seq_t a(idx_t n)
{
    uint64_t primes[64];
    uint64_t exps  [64];
    size_t   cnt;

    /* Factorise n */
    factor_u64(n, primes, exps, &cnt);

    /* Compute ψ(n) = Prod (p^e + p^(e-1)) */
    seq_t psi_n = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t pe   = 1;
        for (size_t j = 0; j < exps[i]; ++j) pe *= (seq_t)primes[i];
        seq_t term = pe + pe / (seq_t)primes[i];
        psi_n *= term;
    }

    /* A342915(n) */
    seq_t target_g = gcd((seq_t)n + 1, psi_n);

    /* Count divisors matching this value */
    idx_t result = 0;
    count_divisors(0, 1, 1, primes, exps, cnt, target_g, &result);
    return (seq_t)result;
}
