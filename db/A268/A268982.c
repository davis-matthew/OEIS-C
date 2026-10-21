/*
 * A268982  a(n) is the denominator of Sum_{d|n} sigma(n/d)^d/d, where sigma
 *          is A000203.
 * https://oeis.org/A268982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = n / gcd( Σ_{d|n} σ(n/d)^d * (n/d) , n )
 * Range:     u8 n <= 219, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, mod_pow, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/sigma_of.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t n_seq = (seq_t)n;
    seq_t sum_mod = 0;

    for (idx_t d = 1; d <= n; ++d) {
        if (n % d != 0) continue;
        idx_t m = n / d;              /* m = n/d */
        seq_t sigma = sigma_of(m);    /* σ(m) */
        seq_t sigma_mod = sigma % n_seq;
        seq_t pow_mod = mod_pow(sigma_mod, d, n_seq);      /* σ(m)^d (mod n) */
        seq_t term_mod = (pow_mod * (seq_t)m) % n_seq;     /* times m , modulo n */
        sum_mod += term_mod;
        if (sum_mod >= n_seq) sum_mod -= n_seq;           /* keep modulo n */
    }

    seq_t g = gcd(sum_mod, n_seq);
    return n_seq / g;
}
