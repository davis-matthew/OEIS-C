/*
 * A347957  Dirichlet convolution of A001221 (omega) with A003602 (a
 *          Kimberling paraphrase).
 * https://oeis.org/A347957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} A001221(n/d) * A003602(d), where A001221 =
 *            omega, A003602(d) = (odd part of d + 1)/2
 * Range:     u8 n <= 329, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 106 terms through n = 9510 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t A001221_omega(idx_t n)
{
    idx_t count = 0;
    if (n <= 1)
        return 0;
    if (n % 2 == 0) {
        count++;
        while (n % 2 == 0)
            n /= 2;
    }
    for (idx_t p = 3; p <= n / p; p += 2) {
        if (n % p == 0) {
            count++;
            while (n % p == 0)
                n /= p;
        }
    }
    if (n > 1)
        count++;
    return count;
}

static idx_t A003602_k(idx_t n)
{
    while ((n & 1) == 0)
        n >>= 1;
    return (n + 1) >> 1;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t d = 1; d <= n / d; d++) {
        if (n % d == 0) {
            idx_t e = n / d;
            sum += (seq_t)A001221_omega(e) * (seq_t)A003602_k(d);
            if (e != d)
                sum += (seq_t)A001221_omega(d) * (seq_t)A003602_k(e);
        }
    }
    return sum;
}
