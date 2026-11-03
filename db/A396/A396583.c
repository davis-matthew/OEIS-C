/*
 * A396583  Triangle read by rows: T(n, k) = binomial(n, k) * A396201(k).
 * https://oeis.org/A396583
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = binomial(r, j) * B(j) where r = floor((sqrt(8n+1)-1)/2),
 *            j = n - r*(r+1)/2, and B(0)=1, B(1)=3, B(2)=-2,
 *            B(k)=(-1)^(k-1)*(|B(k-1)|+|B(k-2)|) for k≥3
 * Range:     i8 n <= 30, i16 n <= 97, i32 n <= 337, i64 n <= 1202,
 *            i128 n <= 4522, big any n
 * Validated: standard, pass, 71 terms through n = 9586 (2026-11-03)
 * Helpers:   binomial, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/mem.h"

/* Memoised computation of B(k) = A396201(k) */
static seq_t get_B(idx_t k)
{
    static seq_t *cache = NULL;
    static idx_t cap = 0;   /* number of computed elements */

    if (k < 3) {
        if (k == 0) return (seq_t)1;
        if (k == 1) return (seq_t)3;
        return (seq_t)-2;   /* k == 2 */
    }

    if (cache == NULL) {
        cap = 3;
        cache = mem_new(seq_t, cap);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)-2;
    }

    if (k >= cap) {
        /* extend cache */
        idx_t new_cap = k + 1;
        cache = mem_resize(seq_t, cache, new_cap);
        for (idx_t i = cap; i <= k; ++i) {
            seq_t a = cache[i - 1];
            seq_t b = cache[i - 2];
            seq_t abs_a = a > (seq_t)0 ? a : -a;
            seq_t abs_b = b > (seq_t)0 ? b : -b;
            seq_t sum = abs_a + abs_b;
            seq_t sign = ((i - 1) & 1) ? (seq_t)-1 : (seq_t)1;
            cache[i] = sum * sign;
        }
        cap = new_cap;
    }
    return cache[k];
}

/* Compute the triangular index r for given n:
   r is such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
static idx_t triangular_row(idx_t n)
{
    idx_t low = 0, high = 1;
    /* find an upper bound */
    while ((__uint128_t)high * (high + 1) / 2 <= n)
        high <<= 1;
    /* binary search */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if ((__uint128_t)mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Determine row r and column j */
    idx_t r = triangular_row(n);
    idx_t tri_r = r * (r + 1) / 2;
    idx_t j = n - tri_r;

    /* binomial coefficient C(r, j) */
    seq_t binom = binomial(r, j);

    /* B(j) */
    seq_t Bj = get_B(j);

    /* Result */
    return binom * Bj;
}
