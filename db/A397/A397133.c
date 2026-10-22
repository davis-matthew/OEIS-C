/*
 * A397133  Triangle T(n,m) read by rows: the number of partitions of [1..n]
 *          into sets of size m.
 * https://oeis.org/A397133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   T(n,m) = n! / ((m!)^k * k!) where k = n/m
 * Range:     u8 n <= 15, u16 n <= 36, u32 n <= 78, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) returns the triangle coefficient in row-major order. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static inline idx_t
row_of_index(idx_t n)
{
    if (n <= 1u)
        return 1;
    idx_t low = 1, high = 2;
    while (high * (high + 1) / 2 < n)
        high <<= 1;
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        if (mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* Multiplicative binomial coefficient a choose b.
 * Uses gcd reductions to keep intermediate values small. */
static seq_t
binomial(idx_t a, idx_t b)
{
    if (b > a - b)
        b = a - b;
    seq_t result = 1;
    for (idx_t i = 1; i <= b; ++i) {
        seq_t num = (seq_t)(a - b + i);
        seq_t den = (seq_t)i;

        seq_t g = gcd(num, den);
        num /= g;
        den /= g;

        g = gcd(result, den);
        result /= g;
        den /= g;     /* after cancellations den should be 1 or a divisor of num */

        result = result * num;
        if (den != 1)
            result /= den;
    }
    return result;
}

seq_t
a(idx_t n)
{
    if (n == 0)
        return 0;            /* outside the offset, value insignificant */

    /* Locate the row (r) and column (c) (1‑based). */
    idx_t r = row_of_index(n);
    idx_t prev_total = (r - 1) * r / 2;
    idx_t c = n - prev_total;   /* 1 ≤ c ≤ r */

    /* The partition exists only if the block size divides the row size. */
    if (r % c != 0u)
        return 0;

    idx_t k = r / c;   /* number of blocks */
    if (k == 0)
        return 0;

    /* Numerator: product of binomial coefficients. */
    seq_t numerator = 1;
    for (idx_t i = 0; i < k; ++i) {
        idx_t a = r - i * c;
        seq_t bin = binomial(a, c);
        numerator = numerator * bin;
    }

    /* Divide by k! using successive gcd reductions. */
    for (idx_t i = 1; i <= k; ++i) {
        seq_t g = gcd(numerator, (seq_t)i);
        numerator /= g;
    }

    return numerator;
}
