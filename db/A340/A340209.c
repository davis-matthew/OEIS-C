/*
 * A340209  Constant whose decimal expansion is the concatenation of the
 *          largest n-digit cube A061435(n), for n = 1, 2, 3, ...
 * https://oeis.org/A340209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, cons
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 9, u32 n <= 44, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   digit_len, floor_cube_root, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of the decimal digits of the concatenation of
   the largest cube having exactly 1, 2, 3, … digits.
   a(0) is the first digit of the 1‑digit cube (8), a(1) the second
   digit of the 1‑digit cube, a(2) the first digit of the 2‑digit cube, etc.
   The function computes the n‑th digit (0‑based) by generating
   the largest cube for each digit length on demand. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/pow10.h"             /* seq_t pow10(idx_t) */
#include "../utils/floor_cube_root.h"   /* seq_t floor_cube_root(seq_t) */
#include "../utils/digit_len.h"         /* size_t digit_len(seq_t) */

static seq_t *cubes = NULL;    /* largest cube for each digit length (1‑based) */
static idx_t *cum   = NULL;    /* cumulative number of digits up to that cube */
static size_t cap   = 0;       /* allocated capacity of the arrays */
static size_t sz    = 0;       /* number of cubes currently known */

/* Extend the tables up to a given digit length (1‑based).  */
static void extend_to(size_t new_len)
{
    while (sz < new_len) {
        idx_t k = (idx_t)(sz + 1);          /* number of digits of the new cube */
        seq_t tenk = pow10(k);              /* 10^k */
        seq_t max  = tenk - (seq_t)1;       /* 10^k – 1 */
        seq_t root = floor_cube_root(max);  /* floor(cuberoot(10^k – 1)) */
        seq_t sq   = root * root;
        seq_t cube = sq * root;             /* cube = root^3 */

        size_t digits = digit_len(cube);    /* decimal digits of this cube */

        if (sz == cap) {                    /* grow storage if needed */
            cap = cap ? cap << 1 : 4;
            cubes = (seq_t *)realloc(cubes, cap * sizeof(seq_t));
            cum   = (idx_t *)realloc(cum,   cap * sizeof(idx_t));
        }

        cubes[sz] = cube;
        cum[sz]   = (sz == 0) ? (idx_t)digits : cum[sz-1] + (idx_t)digits;
        sz++;
    }
}

/* Return the n-th digit of the concatenated sequence. */
seq_t a(idx_t n)
{
    /* Ensure the first cube is available. */
    if (sz == 0) extend_to(1);

    /* Grow until cumulative digits exceed n. */
    while (cum[sz-1] <= n) extend_to(sz + 1);

    /* Binary search to find the cube containing position n. */
    idx_t lo = 0, hi = sz - 1;
    while (lo < hi) {
        idx_t mid = (lo + hi) >> 1;
        if (cum[mid] > n) hi = mid;
        else             lo = mid + 1;
    }
    idx_t cube_idx = lo;

    /* Position inside the selected cube. */
    idx_t prev_digits = (cube_idx == 0) ? (idx_t)0 : cum[cube_idx-1];
    idx_t offset      = n - prev_digits;            /* 0‑based offset */

    seq_t cube         = cubes[cube_idx];
    size_t digits      = digit_len(cube);
    idx_t exp          = (idx_t)(digits - offset - 1); /* 10^exp */
    seq_t divisor      = pow10(exp);
    seq_t temp         = cube / divisor;
    seq_t digit        = temp % (seq_t)10;

    return digit;
}
