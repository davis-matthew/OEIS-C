/*
 * A389074  Numerator of ratio A034448(n) / A048250(n), where A034448 is the
 *          sum of unitary divisors and A048250 is the sum of squarefree
 *          divisors.
 * https://oeis.org/A389074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of ( Π_{p^e‖n} (1+p^e) ) / ( Π_{p‖n} (1+p) )
 *            after full cancellation
 * Range:     u8 n <= 255, u16 n <= 9843, u32 n <= 9843, u64 n <= 9843,
 *            u128 n <= 9843, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Factor n into prime powers.  The helper can handle any 64‑bit n. */
    static uint64_t primes[64];
    static uint64_t exps[64];
    size_t pf_cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &pf_cnt);

    seq_t num = 1;   /* current numerator */
    seq_t den = 1;   /* current denominator */

    for (size_t i = 0; i < pf_cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t exp = exps[i];

        /* Compute A = 1 + p^exp,   B = 1 + p */
        seq_t pow_val = 1;
        for (uint64_t j = 0; j < exp; ++j) {
            pow_val *= (seq_t)p;
        }
        seq_t A = pow_val + 1;
        seq_t B = (seq_t)p + 1;

        /* Reduce A and B locally:  g = gcd(A,B) */
        seq_t g_local = gcd(A, B);
        seq_t A1 = A / g_local;
        seq_t B1 = B / g_local;          /* A1 and B1 are coprime */

        /* Cross cancellation with the running fraction */
        seq_t gA = gcd(A1, den);
        seq_t A2 = A1 / gA;               /* cancel parts common with den   */
        den /= gA;

        seq_t gB = gcd(B1, num);
        seq_t B2 = B1 / gB;               /* cancel parts common with num   */
        num /= gB;

        num *= A2;                        /* multiply reduced numerator */
        den *= B2;                        /* multiply reduced denominator */
    }

    /* After all primes processed, num and den are coprime.  Return numerator. */
    return num;
}
