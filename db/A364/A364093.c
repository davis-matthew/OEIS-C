/*
 * A364093  Sum of divisors of 5*n-2 of form 5*k+1.
 * https://oeis.org/A364093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Sum of divisors of 5*n-2 that are ≡1 (mod 5)
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Recursive helper that enumerates all divisors of a number given its prime factorisation. */
static void dfs(const uint64_t *factors, const uint8_t *exps, size_t cnt,
                size_t pos, seq_t cur, seq_t *sum)
{
    if (pos == cnt) {
        if ((cur % 5) == 1) {
            *sum += cur;
        }
        return;
    }

    seq_t val = 1;
    for (uint8_t e = 0; e <= exps[pos]; ++e) {
        dfs(factors, exps, cnt, pos + 1, cur * val, sum);
        val *= factors[pos];
    }
}

seq_t a(idx_t n)
{
    idx_t m = 5 * n - 2;

    /* Prime factorisation of m.  The number of distinct prime factors is small. */
    uint64_t factors[64];
    uint8_t exps[64];
    size_t cnt = 0;
    idx_t temp = m;
    idx_t p;

    for (p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            exps[cnt] = 0;
            do {
                temp /= p;
                ++exps[cnt];
            } while (temp % p == 0);
            factors[cnt++] = p;
        }
    }
    if (temp > 1) {
        factors[cnt] = temp;
        exps[cnt] = 1;
        ++cnt;
    }

    seq_t sum = 0;
    dfs(factors, exps, cnt, 0, 1, &sum);
    return sum;
}
