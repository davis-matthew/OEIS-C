/*
 * A321807  a(n) = Sum_{d divides n} (-1)^(d + n/d) * d^10.
 * https://oeis.org/A321807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = Sum_{d|n} (-1)^(d + n/d) * d^10
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 8, i64 n <= 78, i128 n <= 6653,
 *            big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation enumerates all divisors of n by recursive
   prime‑factor traversal. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

/* Recursive enumeration of divisors.
   cur_d holds the product of the primes chosen so far.
   total is the accumulated sum of the signed 10th powers. */
static void _dfs(const uint64_t primes[], const uint64_t exps[],
                 size_t idx, size_t cnt, uint64_t nn,
                 seq_t cur_d, seq_t *total)
{
    if (idx == cnt) {
        uint64_t nd = nn / (uint64_t)cur_d;          /* n / d   */
        seq_t parity = ((cur_d & 1) ^ (nd & 1));     /* (d + n/d) mod 2 */
        seq_t term   = big_pow(cur_d, (seq_t)10);    /* d^10    */
        if (parity) term = -term;                    /* sign     */
        *total += term;
        return;
    }

    /* exponent 0 case – choose no factor of primes[idx] */
    _dfs(primes, exps, idx + 1, cnt, nn, cur_d, total);

    /* walk through positive exponents of primes[idx] */
    seq_t p_pow = 1;
    for (uint64_t e = 1; e <= exps[idx]; ++e) {
        p_pow *= (seq_t)primes[idx];
        seq_t newd = cur_d * p_pow;
        _dfs(primes, exps, idx + 1, cnt, nn, newd, total);
    }
}

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for n=0 – return 0 safely */
        return 0;

    uint64_t nn = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt;

    /* Factor n into primes[] and exps[] */
    factor_u64(nn, primes, exps, &cnt);

    seq_t total = 0;
    _dfs(primes, exps, 0, cnt, nn, 1, &total);
    return total;
}
