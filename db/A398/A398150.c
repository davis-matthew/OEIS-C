/*
 * A398150  a(n) = L1 distance between rows n and n+1 of A027746.
 * https://oeis.org/A398150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = L1 distance between rows n and n+1 of A027746 * (prime
 *            factor multisets padded with 1's at the front), sorted matching
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Factor N into array `f` of length `len`. Elements are sorted ascending. */
static void factor_uint64(uint64_t n, uint64_t *f, size_t *len)
{
    size_t p = 0;
    if (n <= 1) { *len = 0; return; }
    while ((n & 1ULL) == 0ULL) { f[p++] = 2ULL; n >>= 1; }
    for (uint64_t i = 3; i <= n / i; i += 2) {
        while (n % i == 0ULL) {
            f[p++] = i;
            n /= i;
        }
    }
    if (n > 1) f[p++] = n;
    *len = p;
}

/* Computes the L1 distance between the prime factor lists of n and n+1. */
seq_t a(idx_t n)
{
    /* buffers for prime factors of n and n+1 */
    uint64_t f1[64], f2[64];
    size_t l1, l2;
    factor_uint64((uint64_t)n, f1, &l1);
    factor_uint64((uint64_t)(n + 1), f2, &l2);

    size_t m = (l1 > l2) ? l1 : l2;          /* length after padding */
    size_t off1 = m - l1;                   /* pre-pad with 1's */
    size_t off2 = m - l2;

    seq_t sum = 0;
    for (size_t i = 0; i < m; ++i) {
        seq_t v1 = (i < off1) ? (seq_t)1 : (seq_t)f1[i - off1];
        seq_t v2 = (i < off2) ? (seq_t)1 : (seq_t)f2[i - off2];
        sum += (v1 > v2) ? (v1 - v2) : (v2 - v1);
    }
    return sum;
}
