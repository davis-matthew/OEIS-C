/*
 * A309153  a(n) = A000203(n)*A001227(n).
 * https://oeis.org/A309153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = sigma(n)*delta(n) where sigma is the sum of divisors and
 *            * delta(n) is the number of odd divisors of n. The sequence is *
 *            multiplicative with a(2^e)=2^(e+1)-1 and for odd primes p, *
 *            a(p^e)=(e+1)*(p^(e+1)-1)/(p-1)
 * Range:     u8 n <= 16, u16 n <= 1888, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

#include "../utils/factor_u64.h"   /* void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */
#include "../utils/seq_pow.h"     /* seq_t seq_pow(seq_t base, idx_t exp); */

seq_t a(idx_t n)
{
    if (n == 1) return 1;          /* sigma(1)=1, odd-divisor count=1 */

    seq_t sigma      = 1;
    seq_t odd_div_cnt = 1;

    /* Factorise n.  64 primes are enough for any 64‑bit integer.  */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        /* sigma factor: (p^(e+1)-1)/(p-1) */
        seq_t p_pow   = seq_pow((seq_t)p, (idx_t)(e + 1));
        seq_t sigma_f = (p_pow - (seq_t)1) / ((seq_t)p - (seq_t)1);
        sigma *= sigma_f;

        /* delta factor: number of odd divisors = product over odd primes of (e+1) */
        if (p != 2) {
            odd_div_cnt *= ((seq_t)e + (seq_t)1);
        }
    }

    return sigma * odd_div_cnt;
}
