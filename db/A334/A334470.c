/*
 * A334470  a(n) = Product_{d|n} (A253139(n) / tau(d)) where A253139(n) =
 *          lcm_{d|n} tau(d).
 * https://oeis.org/A334470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{d|n} (lcm_{d'|n} τ(d') / τ(d)) where τ(d) = number of
 *            divisors of d. This implementation computes all divisors of n by
 *            trial division, then finds the lcm of their τ values and finally
 *            forms the product
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 47, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/gcd.h"

static void factorize_uint64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt)
{
    size_t c = 0;
    uint64_t d = 2;
    while (d * d <= n) {
        if (n % d == 0) {
            uint64_t e = 0;
            while (n % d == 0) {
                n /= d;
                e++;
            }
            pr[c] = d;
            ex[c] = e;
            c++;
        }
        d++;
    }
    if (n > 1) {
        pr[c] = n;
        ex[c] = 1;
        c++;
    }
    *cnt = c;
}

/* Recursive generation of divisors and their τ(d) values.  */
static void gen_divisors(const uint64_t *pr, const uint64_t *ex,
                         size_t num, size_t idx,
                         seq_t cur, seq_t tau_val,
                         seq_t *divs, seq_t *tau_divs, size_t *pos)
{
    if (idx == num) {
        divs[*pos]     = cur;
        tau_divs[*pos] = tau_val;
        (*pos)++;
        return;
    }
    uint64_t p = pr[idx];
    uint64_t e = ex[idx];
    seq_t p_pow = 1;
    for (size_t k = 0; k <= e; ++k) {
        seq_t new_tau = tau_val * (seq_t)(k + 1);
        gen_divisors(pr, ex, num, idx + 1,
                     cur * p_pow, new_tau,
                     divs, tau_divs, pos);
        p_pow *= (seq_t)p;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined for n < 1 */

    /* Factorise n. 10000 factors fit in at most 6 prime factors. */
    uint64_t primes[20];
    uint64_t exps[20];
    size_t   pcnt;
    factorize_uint64((uint64_t)n, primes, exps, &pcnt);

    /* Buffers for divisors and τ(d) values.  N <= 10_000 has <= 512 divisors. */
    seq_t divs[512];
    seq_t tau_divs[512];
    size_t pos = 0;

    gen_divisors(primes, exps, pcnt,
                 0, (seq_t)1, (seq_t)1,
                 divs, tau_divs, &pos);

    /* Compute L = lcm of all τ(d). */
    seq_t L = tau_divs[0];
    for (size_t i = 1; i < pos; ++i) {
        seq_t g = gcd(L, tau_divs[i]);
        L = (L / g) * tau_divs[i];
    }

    /* Compute product ∏ (L / τ(d)). */
    seq_t result = 1;
    for (size_t i = 0; i < pos; ++i) {
        result *= (L / tau_divs[i]);
    }

    return result;
}
