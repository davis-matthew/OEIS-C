/*
 * A379119  a(1) = 1; for n > 1, a(n) is the smallest unitary divisor d > 1 of
 *          n such that A048720(A065621(sigma(n/d)),sigma(d)) is equal to
 *          sigma(n).
 * https://oeis.org/A379119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest unitary divisor d>1 of n with
 *            A048720(A065621(sigma(n/d)),sigma(d)) = sigma(n); a(1)=1.
 *            Computed by enumerating unitary divisors
 * Range:     u8 n <= 254, u16 n <= 65535, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 105 terms through n = 9600 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t gf2_mul_s(seq_t i, seq_t j)
{
    seq_t result = 0;
    seq_t shifted = i;
    while (j != 0) {
        if ((j & 1) != 0)
            result ^= shifted;
        shifted <<= 1;
        j >>= 1;
    }
    return result;
}

static seq_t rev_bin_s(seq_t x)
{
    seq_t low = x ^ (x & (x - 1));
    return x ^ ((x - low) << 1);
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;
    idx_t pp[24];
    seq_t sg[24];
    int k = 0;
    idx_t m = n;
    for (idx_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            idx_t q = 1;
            seq_t s = 1;
            seq_t pw = 1;
            while (m % p == 0) {
                m /= p;
                q *= p;
                pw *= (seq_t)p;
                s += pw;
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

    idx_t best = n;
    unsigned long total = 1UL << k;
    for (unsigned long mask = 1; mask < total; mask++) {
        idx_t d = 1;
        seq_t sd = 1, so = 1;
        for (int i = 0; i < k; i++) {
            if ((mask >> i) & 1UL) {
                d *= pp[i];
                sd *= sg[i];
            } else {
                so *= sg[i];
            }
        }
        if (d >= best) continue;
        if (gf2_mul_s(rev_bin_s(so), sd) == sn)
            best = d;
    }
    return (seq_t)best;
}
