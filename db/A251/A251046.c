/*
 * A251046  Squarefree part of A098548(n).
 * https://oeis.org/A251046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A007913(A098548(n)); A098548: a(1..3)=1,2,3,
 *            a(n)=smallest m>a(n-1) with gcd(m,a(n-2))>1 and gcd(m,a(n-1))=1
 *            (search)
 * Range:     u8 n <= 68, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 9200 (2026-11-07)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

static seq_t A098548_a(idx_t n)
{
    if (n <= 3)
        return (seq_t)n;
    seq_t prev2 = (seq_t)2;
    seq_t prev1 = (seq_t)3;
    for (idx_t k = 4; k <= n; ++k) {
        seq_t c = prev1 + 1;
        for (;;) {
            if (gcd(c, prev2) != (seq_t)1 && gcd(c, prev1) == (seq_t)1)
                break;
            ++c;
        }
        prev2 = prev1;
        prev1 = c;
    }
    return prev1;
}

static seq_t A007913_a(seq_t temp)
{
    seq_t result = 1;
    seq_t i = 2;
    while (i <= temp / i) {
        seq_t i_sq = i * i;
        while (temp % i_sq == 0)
            temp = temp / i_sq;
        if (temp % i == 0) {
            result = result * i;
            temp = temp / i;
        }
        i++;
    }
    if (temp > 1)
        result = result * temp;
    return result;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;
    return A007913_a(A098548_a(n));
}
