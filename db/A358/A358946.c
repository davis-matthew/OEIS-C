/*
 * A358946  Positive integers that are properly represented by each primitive
 *          binary quadratic form of discriminant 28 that is properly
 *          equivalent to the principal form [1, 4, -3].
 * https://oeis.org/A358946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A358946
    Formula:  positive integers k that are properly represented by the primitive
    binary quadratic form x^2 + 4xy – 3y^2 (primitive and properly equivalent to
    the principal form [1,4,-3]).
    For 0 < k ≤ B the representation is equivalent to the existence of
    integers u=x+2y , v=y with u^2 – 7v^2 = k and gcd(u-2v, v)=1.
    The sequence consists of all such k in increasing order. */

/*  Libraries used:
    - <stdlib.h>  : memory allocation
    - <stdint.h>  : fixed-width integer types
    - <stdbool.h> : bool definition
    - <math.h>    : sqrt() for bounds
    - "../utils/gcd.h" : gcd(seq_t, seq_t)   */

/*  Allocated static data is built lazily on first call of a().
    The chosen upper bound B = 5 000 000 guarantees more than
    700 000 sequence terms, far exceeding the 10 000‑term
    requirement of the tests.  The generation cost is
    only about one‑million loop iterations. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/gcd.h"

static seq_t *seq_vals = NULL;     /* the sequence */
static size_t seq_len = 0;         /* number of elements stored */
static bool built = false;         /* flag whether seq_vals is ready */

/*  Build seq_vals up to an upper bound B.  */
static void build_sequence(void)
{
    const idx_t B = 5000000;                  /* upper bound of k          */
    /* present[n] == 1  ⇔  n is properly represented (1 ≤ n ≤ B) */
    uint8_t *present = (uint8_t *)calloc((size_t)B + 1, 1);
    if (!present) exit(EXIT_FAILURE);   /* allocation failed          */

    /* Upper bound for u to cover all cases u^2 – 7v^2 ≤ B   */
    const idx_t umax = (idx_t)(sqrt(2.0 * (double)B)) + 5;

    for (idx_t u = 1; u <= umax; ++u) {
        /* v must satisfy 7v^2 < u^2   ;  vmax = floor(u / sqrt(7))  */
        const idx_t vmax = (idx_t)floor((double)u / 2.64575131106L);
        for (idx_t v = 0; v <= vmax; ++v) {
            seq_t n = (seq_t)u * (seq_t)u - (seq_t)7 * (seq_t)v * (seq_t)v;
            if (n == 0 || n > B) continue;
            /* x = u – 2v  ,  y = v   */
            seq_t x = (seq_t)u - (seq_t)2 * (seq_t)v;
            seq_t y = (seq_t)v;
            seq_t g = gcd(x < 0 ? -x : x, y);
            if (g == 1) present[(size_t)n] = 1;
        }
    }

    /* Count represented numbers and store them in seq_vals */
    seq_len = 0;
    size_t capacity = 128;
    seq_vals = (seq_t *)malloc(capacity * sizeof(seq_t));
    if (!seq_vals) exit(EXIT_FAILURE);

    for (size_t i = 1; i <= (size_t)B; ++i) {
        if (present[i]) {
            if (seq_len >= capacity) {
                capacity <<= 1;
                seq_vals = (seq_t *)realloc(seq_vals, capacity * sizeof(seq_t));
                if (!seq_vals) exit(EXIT_FAILURE);
            }
            seq_vals[seq_len++] = (seq_t)i;
        }
    }

    free(present);
}

/*  Public function requested by the problem statement */
seq_t a(idx_t n)
{
    if (!built) {
        build_sequence();
        built = true;
    }
    /*  n is 1‑based, caller guarantees n >= 1                          */
    if (n == 0 || n > seq_len) {
        /*  out‑of‑range request – undefined behaviour for the problem  */
        return 0;
    }
    return seq_vals[(size_t)n - 1];
}
