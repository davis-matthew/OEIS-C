/*
 * A387788  Maximum value of row n of table A387615.
 * https://oeis.org/A387788
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)*A079277(n), where A079277(n) is the largest k < n
 *            whose prime factors all divide n
 * Range:     u8 n <= 17, u16 n <= 257, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Validated: standard, pass, 75 terms through n = 1842 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

static idx_t A387788_lpk(idx_t n)
{
    idx_t primes[64];
    int pcount = 0;
    idx_t tmp = n;
    for (idx_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            primes[pcount++] = p;
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1) primes[pcount++] = tmp;

    for (idx_t k = n - 1; k > 0; --k) {
        idx_t v = k;
        for (int i = 0; i < pcount; ++i) {
            idx_t p = primes[i];
            while (v % p == 0) v /= p;
        }
        if (v == 1) return k;
    }
    return 1;
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;
    idx_t k = A387788_lpk(n);
    return (seq_t)(n - 1) * (seq_t)k;
}
