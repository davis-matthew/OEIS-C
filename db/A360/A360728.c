/*
 * A360728  Expansion of Sum_{k>=0} (k * x * (1 + x^3))^k.
 * https://oeis.org/A360728
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/4)} (n-3*k)^(n-3*k) * binomial(n-3*k,
 *            k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "../utils/big_pow.h"      /* seq_t big_pow(seq_t base, seq_t exp); */
#include "../utils/binomial.h"     /* seq_t binomial(idx_t a, idx_t b); */

seq_t a(idx_t n)
{
    seq_t sum = (seq_t)0;
    idx_t maxk = n / 4;                    /* floor(n/4) */
    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t m = n - 3 * k;               /* m = n - 3*k */
        seq_t power = big_pow((seq_t)m, (seq_t)m);   /* m^m  */
        seq_t coeff = binomial(m, k);                  /* C(m,k) */
        sum += power * coeff;
    }
    return sum;
}
