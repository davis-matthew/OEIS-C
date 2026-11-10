/*
 * A307239  Analog of Pascal's triangle, with A007947 applied to each sum.
 * https://oeis.org/A307239
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = A007947(T(n-1,k-1) + T(n-1,k)), T(n,0) = T(n,n) = 1;
 *            a(n(n+1)/2 + k) = T(n,k)
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Validated: partial, pass, 91 terms through n = 90 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

static seq_t A007947_a(seq_t m) {
    seq_t result = 1;
    if (m % 2 == 0) {
        result *= 2;
        while (m % 2 == 0) m /= 2;
    }
    for (seq_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            result *= p;
            while (m % p == 0) m /= p;
        }
    }
    if (m > 1) result *= m;
    return result;
}

seq_t a(idx_t n) {
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) r++;
    idx_t k = n - r * (r + 1) / 2;

    seq_t *row = mem_new(seq_t, (size_t)r + 2);
    row[0] = 1;
    for (idx_t i = 1; i <= r; i++) {
        row[i] = 0;
        for (idx_t j = i; j >= 1; j--) {
            row[j] = A007947_a(row[j - 1] + row[j]);
        }
    }
    seq_t res = row[k];
    mem_free(row);
    return res;
}
