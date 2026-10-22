/*
 * A342016  Difference between the arithmetic derivative of A276086(n) and
 *          A276086(n) itself, which is the prime product form of primorial
 *          base expansion of n.
 * https://oeis.org/A342016
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = A168036(A276086(n)) = A327860(n) - A276086(n)
 * Range:     i8 n <= 14, i16 n <= 57, i32 n <= 208, i64 n <= 2278,
 *            i128 n <= 2310, big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* That is, a(n) = Σ_i e_i * m / p_i  -  m ,
 *   where m = Π_i p_i ^ e_i is the prime‑product of the primorial base
 *   expansion of n, and e_i is the corresponding digit. */

#include <stdint.h>
#include "../utils/seq_mul.h"   /* seq_mul(seq_t a, seq_t b) */
#include "../utils/seq_pow.h"   /* seq_pow(seq_t base, idx_t exp) */

static const uint32_t _primes[] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
    31, 37, 41, 43, 47, 53, 59, 61, 67, 71
};
#define _PRIMES_CNT (sizeof(_primes)/sizeof(_primes[0]))

seq_t a(idx_t n)
{
    /* digits of n in the primorial base (e_i's) */
    idx_t digits[_PRIMES_CNT];
    for (size_t k = 0; k < _PRIMES_CNT; ++k) digits[k] = 0;

    /* primorial base expansion of n and the value m = Π p_i ^ e_i */
    seq_t m   = 1;                /* product ∏ p_i^e_i */
    seq_t w   = 1;                /* current weight w_i = ∏_{j<i} p_j */
    size_t pos = 0;               /* next prime index */

    while (w <= (seq_t)n && pos < _PRIMES_CNT) {
        seq_t q = (seq_t)n / w;           /* floor(n / w_i) */
        idx_t digit = (idx_t)(q % _primes[pos]);  /* digit e_i < p_i */
        digits[pos] = digit;

        if (digit) {
            seq_t pow_val = seq_pow((seq_t)_primes[pos], digit);
            m = seq_mul(m, pow_val);
        }
        w = w * (seq_t)_primes[pos];      /* next weight w_{i+1} */
        ++pos;
    }

    /* arithmetic derivative: Σ e_i * m / p_i */
    seq_t deriv = 0;
    for (size_t j = 0; j < pos; ++j) {
        idx_t e = digits[j];
        if (!e) continue;
        seq_t term = seq_mul((seq_t)e, m / (seq_t)_primes[j]);
        deriv += term;
    }

    return deriv - m;
}
