/*
 * A379113  a(1) = 1; for n > 1, a(n) is the greatest proper unitary divisor d
 *          of n such that A048720(A065621(sigma(d)),sigma(n/d)) is equal to
 *          sigma(n).
 * https://oeis.org/A379113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; a(n) = greatest proper unitary divisor d of n with
 *            A048720(A065621(sigma(d)),sigma(n/d)) = sigma(n); found by
 *            enumerating unitary divisors
 * Range:     u8 n <= 89, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 117 terms through n = 9741 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t gf2mul_s(seq_t x, seq_t y)
{
    seq_t r = 0;
    while (y > 0) {
        if ((y & 1) != 0) r ^= x;
        x <<= 1;
        y >>= 1;
    }
    return r;
}

static seq_t rev_alt(seq_t x)
{
    if (x == 0) return 0;
    seq_t low = 1;
    while ((x & low) == 0) low <<= 1;
    return x ^ ((x - low) << 1);
}

seq_t a(idx_t n)
{
    if (n <= 1) return 1;
    idx_t pp[20];
    seq_t sg[20];
    int k = 0;
    idx_t m = n;
    for (idx_t p = 2; p * p <= m; p += (p == 2 ? 1 : 2)) {
        if (m % p == 0) {
            idx_t q = 1;
            seq_t s = 1, t = 1;
            while (m % p == 0) {
                m /= p;
                q *= p;
                t *= (seq_t)p;
                s += t;
            }
            pp[k] = q;
            sg[k] = s;
            k++;
        }
    }
    if (m > 1) {
        pp[k] = m;
        sg[k] = (seq_t)m + 1;
        k++;
    }
    seq_t sn = 1;
    for (int i = 0; i < k; i++) sn *= sg[i];
    seq_t best = 1;
    unsigned full = (1u << k) - 1;
    for (unsigned mask = 1; mask < full; mask++) {
        idx_t d = 1;
        seq_t sd = 1, se = 1;
        for (int i = 0; i < k; i++) {
            if (mask & (1u << i)) { d *= pp[i]; sd *= sg[i]; }
            else se *= sg[i];
        }
        if ((seq_t)d > best) {
            if (gf2mul_s(rev_alt(sd), se) == sn) best = (seq_t)d;
        }
    }
    return best;
}
