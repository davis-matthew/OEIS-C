/*
 * A307393  Square array A(n,k), n >= 0, k >= 1, read by antidiagonals, where
 *          column k is the expansion of g.f. ((1-x)^(k-4))/((1-x)^k-x^k).
 * https://oeis.org/A307393
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the (n‑th) term of the square array A(n,k) read by
 *            antidiagonals, * where A(n,k) = sum_{j=0..⌊n/k⌋} binomial(n+3,
 *            k*j+3)
 * Range:     u8 n <= 26, u16 n <= 118, u32 n <= 494, u64 n <= 2014,
 *            u128 n <= 8126, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The following implementation uses a numerically stable binomial
 * computation that works for both 64‑bit integer and arbitrary‑precision
 * builds.  For plain C (64‑bit) it employs __int128 intermediate
 * multiplication to avoid overflow; for C++ builds (when seq_t is an
 * arbitrary‑precision class) it uses ordinary arithmetic. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* C++ builds: seq_t is an arbitrary‑precision type.  
 * Plain C builds: seq_t is an unsigned integer type. */
#if defined(__cplusplus)
static inline seq_t binom_calc(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t num = n - k + i;
        seq_t den = i;
        seq_t g = gcd(res, den);
        res /= g;
        den /= g;
        g = gcd(num, den);
        num /= g;
        den /= g;
        res = res * num;
        res = res / den;
    }
    return res;
}
#else   /* Plain C: seq_t is unsigned integral type */
static inline seq_t binom_calc(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t num = n - k + i;
        seq_t den = i;
        seq_t g = gcd(res, den);
        res /= g;
        den /= g;
        g = gcd(num, den);
        num /= g;
        den /= g;
        unsigned __int128 temp = (unsigned __int128)res * num;
        temp /= den;
        res = (seq_t)temp;
    }
    return res;
}
#endif

seq_t a(idx_t n)
{
    /* Find diagonal d such that cumulative terms up to d-1 <= n < cumulative up to d. */
    idx_t d = 0;
    while (((d + 1) * (d + 2)) / 2 <= n) {
        ++d;
    }

    /* offset within diagonal */
    const idx_t prev = d * (d + 1) / 2;
    const idx_t offset = n - prev;

    const idx_t row = offset;                 /* n in A(n,k) */
    const idx_t col = d - offset + 1;         /* k in A(n,k) */

    /* Compute A(row, col) = sum_{j=0..floor(row/col)} binomial(row+3, col*j+3). */
    seq_t sum = 0;
    const idx_t maxj = row / col;
    for (idx_t j = 0; j <= maxj; ++j) {
        sum += binom_calc(row + 3, col * j + 3);
    }
    return sum;
}
