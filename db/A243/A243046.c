/*
 * A243046  Number of solutions to k*n/(k+n) = x and k*n/(k-n) = y for
 *          integers x and y and natural number k.
 * https://oeis.org/A243046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9927, u16 n <= 9927, u32 n <= 9927, u64 n <= 9927,
 *            u128 n <= 9927, big any n
 * Helpers:   _gcd_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A243046: a(n) = number of integers k > 0, k ≠ n, such that
 *     k*n/(k+n) and k*n/(k-n) are both integers.
 * This implementation enumerates pairs of divisors (b,s) of n with
 * s > b, sets a = s - b, checks that |s-2b| divides n and that
 * gcd(a,b)=1. Each such pair gives one valid k = n*a / b. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/_gcd_idx.h"

/* Euclidean algorithm for idx_t (uint64_t) */

seq_t a(idx_t n)
{
    /* Build divisor list for n */
    size_t cap = 32;
    idx_t *divs = (idx_t *)malloc(cap * sizeof(idx_t));
    if (!divs) return 0;          /* allocation failure → 0 */
    size_t m = 0;

    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            if (m == cap) {
                cap <<= 1;
                idx_t *tmp = (idx_t *)realloc(divs, cap * sizeof(idx_t));
                if (!tmp) { free(divs); return 0; }
                divs = tmp;
            }
            divs[m++] = i;
            idx_t j = n / i;
            if (j != i) {
                if (m == cap) {
                    cap <<= 1;
                    idx_t *tmp = (idx_t *)realloc(divs, cap * sizeof(idx_t));
                    if (!tmp) { free(divs); return 0; }
                    divs = tmp;
                }
                divs[m++] = j;
            }
        }
    }

    seq_t count = 0;
    for (size_t i = 0; i < m; ++i) {
        idx_t b = divs[i];
        for (size_t j = i + 1; j < m; ++j) {
            idx_t s = divs[j];
            if (s <= b) continue;          /* ensure s > b */

            idx_t a_val = s - b;            /* a = s - b > 0 */
            if (a_val == 0 || a_val == b) continue;
            /* |s - 2b| must divide n */
            idx_t diff = (s > 2 * b) ? (s - 2 * b) : (2 * b - s);
            if (diff == 0) continue;
            if (n % diff != 0) continue;
            if (_gcd_idx(a_val, b) != 1) continue;
            ++count;
        }
    }

    free(divs);
    return count;
}
