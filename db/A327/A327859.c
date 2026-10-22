/*
 * A327859  a(n) = A276086(A003415(n)), where A003415 is the arithmetic
 *          derivative, and A276086 is the primorial base exp-function.
 * https://oeis.org/A327859
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A276086(A003415(n))
 * Range:     u8 n <= 17, u16 n <= 47, u32 n <= 255, u64 n <= 2639,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

#include "../utils/factor_u64.h"   /* factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) */
#include "../utils/big_pow.h"     /* seq_t big_pow(seq_t base, seq_t exp) */

/* Arithmetic derivative A003415 */
static seq_t arithmetic_derivative(idx_t n)
{
    if (n < 2ULL)
        return 0;   /* d(0) = d(1) = 0 */

    seq_t result = 0;
    uint64_t temp = (uint64_t)n;

    /* factorisation of n */
    uint64_t pr[64], ex[64];
    size_t cnt;
    factor_u64(temp, pr, ex, &cnt);

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)pr[i];
        seq_t e = (seq_t)ex[i];
        result += ((seq_t)n * e) / p;
    }
    return result;
}

/* Primorial‑base exp‑function A276086 */
static seq_t primorial_exp(seq_t m)
{
    /* first 32 primes – enough for all n up to ~10 000 */
    static const uint32_t primes[] = {
        2U, 3U, 5U, 7U, 11U, 13U, 17U, 19U,
        23U, 29U, 31U, 37U, 41U, 43U, 47U, 53U,
        59U, 61U, 67U, 71U, 73U, 79U, 83U, 89U,
        97U, 101U, 103U, 107U, 109U, 113U, 127U
    };
    const size_t prime_count = sizeof(primes) / sizeof(primes[0]);

    seq_t result = 1;
    size_t idx = 0;
    while (m != 0 && idx < prime_count) {
        uint32_t p = primes[idx++];
        seq_t digit = m % (seq_t)p;   /* 0 ≤ digit < p */
        if (digit != 0) {
            seq_t pw = big_pow((seq_t)p, digit);
            result *= pw;
        }
        m /= (seq_t)p;
    }
    /* For the range tested (n ≤ 10 000) m will become zero before the
       loop runs out of primes. */
    return result;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    seq_t d = arithmetic_derivative(n);
    return primorial_exp(d);
}
