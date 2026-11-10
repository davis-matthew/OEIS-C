/*
 * A349391  Dirichlet convolution of A126760 with omega.
 * https://oeis.org/A349391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} A126760(n/d) * A001221(d)
 * Range:     u8 n <= 699, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 108 terms through n = 9444 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A126760_a(idx_t n)
{
    seq_t result = 0;
    idx_t m = n;
    while (m > 0) {
        idx_t r = m % 6;
        if (r == 1) {
            result = ((seq_t)2 * (seq_t)((m - 1) / 6)) + 1;
            break;
        }
        if (r == 5) {
            result = ((seq_t)2 * (seq_t)((m - 5) / 6)) + 2;
            break;
        }
        if ((m & 1) == 0) { m >>= 1; continue; }
        if (m % 3 == 0) { m /= 3; continue; }
        break;
    }
    return result;
}

static seq_t A001221_a(idx_t n)
{
    if (n <= 1)
        return 0;
    seq_t count = 0;
    idx_t temp = n;
    if (temp % 2 == 0) {
        count++;
        while (temp % 2 == 0)
            temp /= 2;
    }
    for (idx_t d = 3; d <= temp / d; d += 2) {
        if (temp % d == 0) {
            count++;
            while (temp % d == 0)
                temp /= d;
        }
    }
    if (temp > 1)
        count++;
    return count;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= n / i; i++) {
        if (n % i != 0)
            continue;
        idx_t j = n / i;
        /* d = i, n/d = j */
        sum += A126760_a(j) * A001221_a(i);
        if (j != i)
            sum += A126760_a(i) * A001221_a(j);
    }
    return sum;
}
