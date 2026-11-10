/*
 * A300834  a(n) = Product_{d|n, d<n} A019565(A003714(d)), where A003714(n) is
 *          the n-th Fibbinary number.
 * https://oeis.org/A300834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{d|n, d<n} A019565(A003714(d)), where A003714 is
 *            the Fibbinary numbers and A019565(m) = Product_{k in bits of m}
 *            prime(k+1)
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 47, u64 n <= 119,
 *            u128 n <= 359, big any n
 * Validated: standard, pass, 78 terms through n = 3143 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint32_t A300834_primes[64] = {
    2, 3, 5, 7, 11, 13, 17, 19,
    23, 29, 31, 37, 41, 43, 47, 53,
    59, 61, 67, 71, 73, 79, 83, 89,
    97, 101, 103, 107, 109, 113, 127, 131,
    137, 139, 149, 151, 157, 163, 167, 173,
    179, 181, 191, 193, 197, 199, 211, 223,
    227, 229, 233, 239, 241, 251, 257, 263,
    269, 271, 277, 281, 283, 293, 307, 311
};

/* A003714: Zeckendorf digits of n read as binary */
static uint64_t A300834_fibbinary(uint64_t n)
{
    uint64_t f[92];
    int k = 2, i;
    uint64_t rem = n, result = 0;
    f[0] = 1;
    f[1] = 2;
    while (k < 92 && f[k - 1] <= rem) {
        f[k] = f[k - 1] + f[k - 2];
        k++;
    }
    for (i = k - 1; i >= 0; i--) {
        result <<= 1;
        if (f[i] <= rem) {
            rem -= f[i];
            result |= 1;
        }
    }
    return result;
}

/* A019565: product of prime(bit+1) over set bits */
static seq_t A300834_sqfree(uint64_t m)
{
    seq_t r = 1;
    int bit = 0;
    while (m != 0 && bit < 64) {
        if (m & 1ULL) r *= (seq_t)A300834_primes[bit];
        m >>= 1;
        bit++;
    }
    return r;
}

seq_t a(idx_t n)
{
    seq_t result = 1;
    idx_t d;
    for (d = 1; d < n; d++) {
        if (n % d == 0) {
            result *= A300834_sqfree(A300834_fibbinary((uint64_t)d));
        }
    }
    return result;
}
