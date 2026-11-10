/*
 * A316850  The table in A316842 with columns concatenated to form a single
 *          number.
 * https://oeis.org/A316850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = decimal concatenation of the digits of i, j, k, where
 *            (i,j,k) is the n-th row of A316842 (i>=j>=k>=1, j+k>i,
 *            gcd(i,j,k)=1, rows in order of increasing i, then j, then k);
 *            found by direct search
 * Range:     u8 n <= 2, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A316850_gcd(uint64_t a, uint64_t b)
{
    while (b) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

static seq_t A316850_pow10_digits(uint64_t x)
{
    seq_t p = 1;
    do {
        p *= 10;
        x /= 10;
    } while (x);
    return p;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    uint64_t count = 0;
    for (uint64_t i = 1;; i++) {
        for (uint64_t j = 1; j <= i; j++) {
            uint64_t k0 = (i >= j) ? (i - j + 1) : 1;
            for (uint64_t k = k0; k <= j; k++) {
                if (A316850_gcd(A316850_gcd(i, j), k) != 1)
                    continue;
                count++;
                if (count == (uint64_t)n) {
                    seq_t pk = A316850_pow10_digits(k);
                    seq_t pj = A316850_pow10_digits(j);
                    return (seq_t)i * pj * pk + (seq_t)j * pk + (seq_t)k;
                }
            }
        }
    }
}
