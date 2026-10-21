/*
 * A264786  Let { d_1, d_2, ..., d_k } be the divisors of n. Then a(n) = d_k^1
 *          + d_(k-1)^2 + ... + d_1^k.
 * https://oeis.org/A264786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = d_k^1 + d_{k-1}^2 + ... + d_1^k, where d_i are the
 *            divisors of n sorted in increasing order
 * Range:     u8 n <= 17, u16 n <= 47, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/big_pow.h"


static int cmp_uint64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    return (va > vb) - (va < vb);
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* not needed for valid OEIS index, but safety */

    /* collect divisors of n */
    uint64_t divs[256];
    size_t cnt = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            divs[cnt++] = (uint64_t)i;
            if (i != n / i)
                divs[cnt++] = (uint64_t)(n / i);
        }
    }

    /* sort divisors increasingly */
    qsort(divs, cnt, sizeof(uint64_t), cmp_uint64);

    /* compute the weighted sum d_k^1 + d_{k-1}^2 + ... + d_1^k */
    seq_t result = 0;
    for (size_t idx = 0; idx < cnt; ++idx) {
        seq_t d   = (seq_t)divs[idx];
        seq_t exp = (seq_t)(cnt - idx);     /* largest divisor gets exponent 1 */
        result += big_pow(d, exp);
    }

    return result;
}
