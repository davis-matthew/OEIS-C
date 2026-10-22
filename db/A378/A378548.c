/*
 * A378548  Sum of divisors d of n such that n/d is odd with an even number of
 *          prime factors (counted with multiplicity).
 * https://oeis.org/A378548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, n/d odd, Ω(n/d) even} d
 * Range:     u8 n <= 224, u16 n <= 9700, u32 n <= 9700, u64 n <= 9700,
 *            u128 n <= 9700, big any n
 * Helpers:   factor_u64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

#include "../utils/pow2.h"     /* seq_t pow2(idx_t exp) */
#include "../utils/factor_u64.h" /* void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) */

/* Recursively enumerate all divisors of m using its prime factorisation.
 * For each divisor d, if the parity of Ω(d) matches the target parity
 * we add d to *sum. */
static void dfs_div(size_t idx, seq_t val, int parity,
                    const uint64_t *pr, const uint64_t *ex, size_t cnt,
                    int target, seq_t *sum)
{
    if (idx == cnt) {
        if ((parity & 1) == target)
            *sum += val;
        return;
    }
    seq_t cur = 1;
    for (uint64_t e = 0; e <= ex[idx]; ++e) {
        dfs_div(idx + 1, val * cur, parity ^ (int)(e & 1),
                pr, ex, cnt, target, sum);
        cur *= (seq_t)pr[idx];
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* undefined for n = 0, but guard */

    /* Separate powers of two */
    uint64_t tmp = n;
    int e = 0;
    while ((tmp & 1ULL) == 0) {
        ++e;
        tmp >>= 1;
    }
    uint64_t m = tmp;                   /* odd part of n */

    /* Factorise the odd part */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t prime_cnt = 0;
    factor_u64(m, pr, ex, &prime_cnt);

    /* Parity of Ω(m) */
    int parity_m = 0;
    for (size_t i = 0; i < prime_cnt; ++i)
        parity_m ^= (int)(ex[i] & 1);

    /* Enumerate divisors of m with matching parity */
    seq_t sum_odd = 0;
    dfs_div(0, (seq_t)1, 0, pr, ex, prime_cnt, parity_m, &sum_odd);

    /* Multiply by 2^e (the required factor of all 2's) */
    seq_t pow_e = pow2(e);
    return sum_odd * pow_e;
}
