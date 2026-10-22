/*
 * A338138  Nested cube root (b(n) + (b(n-1) + ... +
 *          (b(1))^(1/3)...)^(1/3))^(1/3), where b(n) = A338137(n).
 * https://oeis.org/A338138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Nested cube root sequence A338138
   a(1) = 1
   a(n) = (a(n-1) + A338137(n))^(1/3)   for n > 1
   where A338137(n) = a(n)^3 – a(n-1) (b(n))
   The implementation memoises computed terms and the set of used
   b(n) values, ensuring uniqueness.  It uses an arbitrary‑precision
   integer type seq_t and uint64_t for indices.  All arithmetic is
   performed with seq_t operators; no floating‑point or <math.h> is
   involved. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_cube_root.h"

/* dynamic array for a(n), 1‑indexed */
static seq_t *a_vals = NULL;
static size_t a_cap = 0;
static idx_t  a_last = 0;          /* last computed index */

/* dynamic array for used b(n) values */
static seq_t *used_b = NULL;
static size_t used_cap = 0;
static size_t used_last = 0;        /* number of used b's */

/* ensure a_vals contains indices 0 … n */
static void a_ensure_capacity(idx_t n)
{
    if (a_vals == NULL) {
        a_cap = 128;
        a_vals = (seq_t *)calloc(a_cap, sizeof(seq_t));
    }
    while ((size_t)n + 1 > a_cap) {
        a_cap <<= 1;
        a_vals = (seq_t *)realloc(a_vals, a_cap * sizeof(seq_t));
    }
}

/* ensure used_b has capacity for at least used_last+1 entries */
static void used_ensure_capacity(void)
{
    if (used_b == NULL) {
        used_cap = 128;
        used_b = (seq_t *)calloc(used_cap, sizeof(seq_t));
    }
    if (used_last >= used_cap) {
        used_cap <<= 1;
        used_b = (seq_t *)realloc(used_b, used_cap * sizeof(seq_t));
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined below offset */

    a_ensure_capacity(n);

    /* initialise sequence with a(1)=1 on first call */
    if (a_last == 0) {
        a_vals[1] = (seq_t)1;            /* a(1) = 1 */
        a_last = 1;

        /* initialise used b(1) = a(1)^3 = 1 */
        used_ensure_capacity();
        used_b[0] = a_vals[1] * a_vals[1] * a_vals[1];
        used_last = 1;
    }

    /* if already computed return */
    if (n <= a_last) return a_vals[n];

    /* compute missing terms up to n */
    for (idx_t k = a_last + 1; k <= n; ++k) {
        seq_t prev = a_vals[k - 1];
        seq_t cand_start = floor_cube_root(prev) + 1; /* smallest integer > prev^{1/3} */
        seq_t cand = cand_start;

        for (;;) {
            seq_t cube = cand * cand * cand;
            seq_t diff = cube - prev;        /* b(k) */

            /* check uniqueness */
            bool used = false;
            for (size_t i = 0; i < used_last; ++i) {
                if (used_b[i] == diff) {
                    used = true;
                    break;
                }
            }

            if (!used) {
                a_vals[k] = cand;
                a_last = k;

                used_ensure_capacity();
                used_b[used_last++] = diff;
                break;
            }
            ++cand;   /* try next integer */
        }
    }

    return a_vals[n];
}
