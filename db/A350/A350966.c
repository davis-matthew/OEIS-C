/*
 * A350966  a(n) = sqrt(28*A296377(n)^2 - 3).
 * https://oeis.org/A350966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sqrt(28*A296377(n)^2 - 3)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mul_small.h"

/* Cached solutions of x^2 - 28*y^2 = -3.
   odd_x/y  : seeds (5,1) for indices n = 1,3,5,...
   even_x/y : seeds (37,7) for indices n = 2,4,6,... */
static seq_t *odd_x = NULL, *odd_y = NULL;
static size_t odd_len = 0, odd_cap = 0;
static seq_t *even_x = NULL, *even_y = NULL;
static size_t even_len = 0, even_cap = 0;

/* Multiply (x,y) by the fundamental unit (127,24) of x^2-28*y^2=1
   to produce the next solution (x',y') of x^2-28*y^2=-3.          */
static void mul_unit(seq_t x, seq_t y, seq_t *nx, seq_t *ny)
{
    /* x' = x*127 + 28*y*24 */
    seq_t a = mul_small(x, 127u);
    seq_t b = mul_small(y, 24u);
    seq_t c = mul_small(b, 28u);
    *nx = a + c;

    /* y' = x*24 + y*127 */
    seq_t d = mul_small(x, 24u);
    seq_t e = mul_small(y, 127u);
    *ny = d + e;
}

/* Ensure that odd_* arrays contain entries 0 … k (k = (n-1)/2).   */
static void ensure_odd(size_t k)
{
    if (k < odd_len) return;

    size_t new_cap = odd_cap ? odd_cap : 1;
    while (new_cap <= k) new_cap <<= 1;

    seq_t *new_x = (seq_t*)realloc(odd_x, new_cap * sizeof(seq_t));
    seq_t *new_y = (seq_t*)realloc(odd_y, new_cap * sizeof(seq_t));
    odd_x = new_x;
    odd_y = new_y;

    if (odd_len == 0) {               /* initialise the seed (5,1) */
        odd_x[0] = (seq_t)5;
        odd_y[0] = (seq_t)1;
        odd_len = 1;
    }

    for (size_t i = odd_len; i <= k; ++i) {
        seq_t nx, ny;
        mul_unit(odd_x[i-1], odd_y[i-1], &nx, &ny);
        odd_x[i] = nx;
        odd_y[i] = ny;
    }

    odd_len = k + 1;
    odd_cap = new_cap;
}

/* Ensure that even_* arrays contain entries 0 … k (k = (n/2)-1).  */
static void ensure_even(size_t k)
{
    if (k < even_len) return;

    size_t new_cap = even_cap ? even_cap : 1;
    while (new_cap <= k) new_cap <<= 1;

    seq_t *new_x = (seq_t*)realloc(even_x, new_cap * sizeof(seq_t));
    seq_t *new_y = (seq_t*)realloc(even_y, new_cap * sizeof(seq_t));
    even_x = new_x;
    even_y = new_y;

    if (even_len == 0) {               /* initialise the seed (37,7) */
        even_x[0] = (seq_t)37;
        even_y[0] = (seq_t)7;
        even_len = 1;
    }

    for (size_t i = even_len; i <= k; ++i) {
        seq_t nx, ny;
        mul_unit(even_x[i-1], even_y[i-1], &nx, &ny);
        even_x[i] = nx;
        even_y[i] = ny;
    }

    even_len = k + 1;
    even_cap = new_cap;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n = 0 */

    if (n & 1u) {                 /* odd index: n = 2k+1 */
        size_t k = (size_t)((n - 1) / 2);
        ensure_odd(k);
        return odd_x[k];
    } else {                      /* even index: n = 2k   */
        size_t k = (size_t)(n / 2) - 1;
        ensure_even(k);
        return even_x[k];
    }
}
