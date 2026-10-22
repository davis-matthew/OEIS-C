/*
 * A354060  Irregular table read by rows: T(n,k) is the number of solutions to
 *          x^k == 1 (mod n), 1 <= k <= psi(n), psi = A002322.
 * https://oeis.org/A354060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = T(n,k) where rows are read sequentially: a(1)=T(1,1),
 *            a(2)=T(2,1), a(3)=T(3,1), a(4)=T(3,2), a(5)=T(4,1),
 * Range:     u8 n <= 715, u16 n <= 8346, u32 n <= 8346, u64 n <= 8346,
 *            u128 n <= 8346, big any n
 * Helpers:   factor_u64, gcd, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"

static seq_t lcm_seq(seq_t a, seq_t b)
{
    seq_t g = gcd(a, b);
    return a / g * b;
}

/* Carmichael function λ(n) */
static seq_t psi_of(idx_t n)
{
    if (n == 1)
        return 1;
    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &cnt);
    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p = pr[i];
        uint64_t e = ex[i];
        seq_t lambda;
        if (p == 2)
        {
            if (e == 1)
                lambda = 1;
            else if (e == 2)
                lambda = 2;
            else
                lambda = ((seq_t)1) << (e - 2); /* λ(2^e) = 2^(e-2) for e ≥ 3 */
        }
        else
        {
            seq_t part = (seq_t)(p - 1);
            seq_t pow = pow_uint(p, (idx_t)(e - 1));
            part *= pow;                    /* λ(p^e) = (p-1)p^(e-1) */
            lambda = part;
        }
        result = lcm_seq(result, lambda);
    }
    return result;
}

/* T(n,k) = product over cyclic components of gcd(k, order_of_component) */
static seq_t T_of(idx_t n, idx_t k)
{
    if (n == 1)
        return 1;
    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &cnt);
    seq_t result = 1;
    seq_t k_seq = (seq_t)k;
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p = pr[i];
        uint64_t e = ex[i];
        if (p == 2)
        {
            if (e == 1)
                continue;          /* trivial group */
            else if (e == 2)
                result *= gcd(k_seq, (seq_t)2);
            else
            {
                result *= gcd(k_seq, (seq_t)2);
                seq_t pow2 = ((seq_t)1) << (e - 2);
                result *= gcd(k_seq, pow2);
            }
        }
        else /* odd prime power, cyclic */
        {
            seq_t order = (seq_t)(p - 1);
            seq_t pow = pow_uint(p, (idx_t)(e - 1));
            order *= pow;            /* order = (p-1)p^(e-1) */
            result *= gcd(k_seq, order);
        }
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) /* behavior below offset is irrelevant */
        return 0;
    seq_t target = (seq_t)n;
    idx_t row = 1;
    seq_t cumulative = 0;
    while (true)
    {
        seq_t len = psi_of(row);
        if (cumulative + len >= target)
        {
            idx_t k = (idx_t)(target - cumulative);
            return T_of(row, k);
        }
        cumulative += len;
        ++row;
    }
}
