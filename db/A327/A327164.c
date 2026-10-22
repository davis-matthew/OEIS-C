/*
 * A327164  Number of iterations of x -> gcd(usigma(x),x) needed to reach a
 *          fixed point, where usigma is the sum of unitary divisors of n
 *          (A034448).
 * https://oeis.org/A327164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1619, u16 n <= 9100, u32 n <= 9100, u64 n <= 9100,
 *            u128 n <= 9100, big any n
 * Helpers:   factor_u64, gcd, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A327164: a(n) = number of iterations of x -> gcd(usigma(x),x)
 * until a fixed point is reached, starting from x = n.
 * usigma(x) = product over prime powers p^a || x of (1 + p^a).
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"

/* Compute the unitary divisor sum of n (n >= 1). */
static seq_t usigma_uint64(uint64_t n)
{
    if (n == 1) return 1;
    uint64_t pr[128], ex[128];
    size_t cnt = 0;
    factor_u64(n, pr, ex, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t part = pow_uint(pr[i], ex[i]);  /* 1 + p^a */
        part = part + 1;
        result = result * part;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* n is guaranteed >= 1 by the contract. */
    uint64_t cur = (uint64_t)n;
    seq_t cnt = 0;

    while (1)
    {
        seq_t cur_s  = (seq_t)cur;
        seq_t us     = usigma_uint64(cur);
        seq_t next   = gcd(cur_s, us);

        if (next == cur_s)
            return cnt;

        cur = (uint64_t)next;
        cnt = cnt + 1;
    }
}
