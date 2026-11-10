/*
 * A363154  Triangle read by rows. The Hadamard product of A173018 and
 *          A349203.
 * https://oeis.org/A363154
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) = A173018(r,k) * A349203(r,k), where n = r(r+1)/2
 *            + k, 0<=k<=r; A173018: T(0,0)=1, T(r,0)=T(r-1,0), T(r,r)=0,
 *            T(r,k)=(k+1)T(r-1,k)+(r-k)T(r-1,k-1); A349203(r,k) =
 *            (lcm(1..r+1)/(r+1)) / binomial(r,k)
 * Range:     u8 n <= 20, u16 n <= 38, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 54 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t A363154_gcd(seq_t x, seq_t y)
{
    while (y != 0) {
        seq_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

seq_t a(idx_t n)
{
    idx_t r = 0;
    while (((idx_t)(r + 1)) * ((idx_t)(r + 2)) / 2 <= n)
        ++r;
    idx_t k = n - r * (r + 1) / 2;

    /* Eulerian triangle entry via in-place row DP */
    seq_t *row = mem_new(seq_t, (size_t)r + 2);
    row[0] = (seq_t)1;
    for (idx_t i = 1; i <= r; ++i) {
        for (idx_t j = i; j >= 1; --j) {
            if (j == i)
                row[j] = (seq_t)0;
            else
                row[j] = (seq_t)(j + 1) * row[j] + (seq_t)(i - j) * row[j - 1];
        }
    }
    seq_t eul = row[k];
    mem_free(row);
    if (eul == 0)
        return (seq_t)0;

    /* L = lcm(1..r+1)/(r+1) */
    seq_t l = (seq_t)1;
    for (idx_t i = 2; i <= r + 1; ++i) {
        seq_t g = A363154_gcd(l, (seq_t)i);
        l = l / g * (seq_t)i;
    }
    l = l / (seq_t)(r + 1);

    /* binomial(r,k) multiplicatively */
    idx_t kk = (k < r - k) ? k : r - k;
    seq_t c = (seq_t)1;
    for (idx_t i = 0; i < kk; ++i) {
        c = c * (seq_t)(r - i) / (seq_t)(i + 1);
    }

    return eul * (l / c);
}
