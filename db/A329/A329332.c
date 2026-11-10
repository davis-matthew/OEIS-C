/*
 * A329332  Table of powers of squarefree numbers, powers of A019565(n) in
 *          increasing order in row n. Square array A(n,k) n >= 0, k >= 0 read
 *          by descending antidiagonals.
 * https://oeis.org/A329332
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A(n,k) = A019565(n)^k, square array read by descending
 *            antidiagonals: a(T(d)+p) = A(p, d-p)
 * Range:     u8 n <= 30, u16 n <= 57, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Validated: partial, pass, 65 terms through n = 64 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

static const uint32_t A329332_primes[64] = {
    2, 3, 5, 7, 11, 13, 17, 19,
    23, 29, 31, 37, 41, 43, 47, 53,
    59, 61, 67, 71, 73, 79, 83, 89,
    97, 101, 103, 107, 109, 113, 127, 131,
    137, 139, 149, 151, 157, 163, 167, 173,
    179, 181, 191, 193, 197, 199, 211, 223,
    227, 229, 233, 239, 241, 251, 257, 263,
    269, 271, 277, 281, 283, 293, 307, 311
};

seq_t a(idx_t n)
{
    idx_t d = (idx_t)((-1.0 + sqrt(1.0 + 8.0 * (double)n)) / 2.0);
    while (d > 0 && d * (d + 1) / 2 > n) d--;
    while ((d + 1) * (d + 2) / 2 <= n) d++;

    idx_t p = n - d * (d + 1) / 2;
    idx_t row = p;
    idx_t k = d - p;

    seq_t base = 1;
    int bit = 0;
    idx_t r = row;
    while (r != 0 && bit < 64) {
        if (r & 1ULL) base *= (seq_t)A329332_primes[bit];
        r >>= 1;
        ++bit;
    }

    seq_t result = 1;
    while (k != 0) {
        if (k & 1ULL) result *= base;
        k >>= 1;
        if (k != 0) base *= base;
    }
    return result;
}
