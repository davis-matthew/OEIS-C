/*
 * A368912  a(n) = 1 if A342001(n) is squarefree, and 0 otherwise.
 * https://oeis.org/A368912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0; for n > 1, a(n) = A008966(A342001(n)), where
 *            A342001(n) = Sum_{p^v || n} v*rad(n)/p
 * Range:     u8 n <= 1964, u16 n <= 100000, u32 n <= 100000, u64 n <= 100000,
 *            u128 n <= 100000, big any n
 * Validated: standard, pass, 137 terms through n = 7411 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static seq_t A342001_val(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;
    idx_t ps[64];
    idx_t es[64];
    size_t cnt = 0;
    idx_t t = n;
    for (idx_t p = 2; p <= t / p; ++p) {
        if (t % p == 0) {
            idx_t e = 0;
            while (t % p == 0) {
                t /= p;
                ++e;
            }
            ps[cnt] = p;
            es[cnt] = e;
            ++cnt;
        }
    }
    if (t > 1) {
        ps[cnt] = t;
        es[cnt] = 1;
        ++cnt;
    }
    seq_t rad = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i)
        rad *= (seq_t)ps[i];
    seq_t result = (seq_t)0;
    for (size_t i = 0; i < cnt; ++i)
        result += (seq_t)es[i] * (rad / (seq_t)ps[i]);
    return result;
}

static int A008966_sqfree(seq_t m)
{
    if (m < 2)
        return 1;
    seq_t temp = m;
    for (seq_t i = 2; i <= temp / i; ++i) {
        if (temp % i == 0) {
            int e = 0;
            do {
                temp /= i;
                ++e;
            } while (temp % i == 0);
            if (e > 1)
                return 0;
        }
    }
    return 1;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;
    return (seq_t)A008966_sqfree(A342001_val(n));
}
