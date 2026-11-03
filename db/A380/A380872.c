/*
 * A380872  Infinite square array, where row r >= 0 is the orbit of r under
 *          the map A380873: concatenate(sum of digits, product of digits).
 * https://oeis.org/A380872
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   a(n) = value at coordinates (r,c) in the infinite array where
 *            rows are orbits of the map M(x)=concat(sumdigits(x),
 *            proddigits(x)), read by antidiagonals. The mapping from index n
 *            to coordinates is r = n - T(s), c = s - r with s = max{k | T(k)
 *            ≤ n} and T(k)=k*(k+1)/2. The value is obtained by iter...
 * Range:     u8 n <= 16, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Validated: partial, pass, 88 terms through n = 87 (2026-11-01)
 * Helpers:   digit_product, digit_sum, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "../utils/digit_product.h"
#include "../utils/digit_sum.h"
#include "../utils/num_digits.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

static inline idx_t triangular(idx_t k)
{
    return k * (k + 1) / 2;
}

static idx_t find_diag(idx_t n)
{
    idx_t lo = 0;
    idx_t hi = 1;
    /* enlarge hi until triangular(hi) > n */
    while (triangular(hi) <= n)
        hi <<= 1;
    /* binary search for largest k with triangular(k) <= n */
    while (lo < hi)
    {
        idx_t mid = lo + (hi - lo) / 2;
        if (triangular(mid) <= n)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1; /* because lo points to first k with T(k) > n */
}

static seq_t orbit_value(idx_t r, idx_t steps)
{
    seq_t cur = (seq_t)r;
    for (idx_t step = 0; step < steps; ++step)
    {
        seq_t ssum = digit_sum(cur);
        seq_t prod = digit_product(cur);
        int plen = num_digits(prod);
        seq_t new_val = seq_mul(ssum, pow10((idx_t)plen)) + prod;
        cur = new_val;
    }
    return cur;
}

seq_t a(idx_t n)
{
    /* determine coordinates (row r, column c) */
    idx_t s = find_diag(n);
    idx_t tk = triangular(s);
    idx_t r = n - tk;
    idx_t c = s - r;

    return orbit_value(r, c);
}
