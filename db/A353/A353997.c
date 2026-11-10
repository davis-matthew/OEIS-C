/*
 * A353997  Partial sums of A078473.
 * https://oeis.org/A353997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} A078473(k), where A078473 is multiplicative
 *            with a(5^e) = (5^(e+1)-1)/4, a(p^e) = 0 for p == 2,3 (mod 5) and
 *            e odd, a(p^e) = (p^(e+2)-1)/(p^2-1) for p == 2,3 (mod 5) and e
 *            even, a(p^e) = Sum_{k=0..e} (k+1)(e-k+1) p^k for p == 1,4 (mod
 *            5)
 * Range:     u8 n <= 30, u16 n <= 451, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 8478 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t A353997_pow(seq_t base, unsigned exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1U) result *= base;
        base *= base;
        exp >>= 1U;
    }
    return result;
}

static seq_t A353997_pf(idx_t p, idx_t e)
{
    if (p == 5) {
        seq_t t = A353997_pow(5, (unsigned)(e + 1));
        return (t - 1) / 4;
    }
    unsigned mod5 = (unsigned)(p % 5);
    if (mod5 == 2 || mod5 == 3) {
        if (e & 1U) return 0;
        seq_t t = A353997_pow((seq_t)p, (unsigned)(e + 2));
        seq_t denom = A353997_pow((seq_t)p, 2U) - 1;
        return (t - 1) / denom;
    }
    seq_t sum = 0;
    seq_t power = 1;
    unsigned ue = (unsigned)e;
    for (unsigned k = 0; k <= ue; ++k) {
        seq_t term = (seq_t)(k + 1) * (seq_t)(ue - k + 1) * power;
        sum += term;
        power *= (seq_t)p;
    }
    return sum;
}

static seq_t A353997_term(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    seq_t result = 1;
    idx_t m = n;
    idx_t i = 2;
    while (i * i <= m) {
        if (m % i == 0) {
            idx_t e = 0;
            while (m % i == 0) {
                m /= i;
                ++e;
            }
            seq_t val = A353997_pf(i, e);
            if (val == 0) return 0;
            result *= val;
        }
        ++i;
    }
    if (m > 1) {
        seq_t val = A353997_pf(m, 1);
        if (val == 0) return 0;
        result *= val;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t s = 0;
    for (idx_t k = 1; k <= n; ++k) {
        s += A353997_term(k);
    }
    return s;
}
