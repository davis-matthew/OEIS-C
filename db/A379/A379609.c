/*
 * A379609  Triples (x,y,z) such that T(x)*T(y) = T(z) with 2 <= x <= y, where
 *          T(n) is the n-th triangular number, sorted first by values of z,
 *          then by values of x.
 * https://oeis.org/A379609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   triplet (x, y, z) such that T(x)*T(y) = T(z) with 2 ≤ x ≤ y *
 *            where T(n) = n*(n+1)/2. * The OEIS sequence is the flattened
 *            list of these triples, * first sorted by z then by x. a(n) is
 *            the n-th entry (1‑based)
 * Range:     u8 none, u16 n <= 24, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

static seq_t *terms = NULL;           /* flattened list of x, y, z, ... */
static size_t terms_cap = 0;          /* allocated elements in terms */
static size_t terms_size = 0;         /* number of elements actually stored */
static idx_t  next_z   = 1;           /* next z value to process */

/* Ensure that the terms array can hold at least `need` elements. */
static void grow_terms(size_t need)
{
    if (need <= terms_cap) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);                /* allocation failure: abort */
    terms = tmp;
    terms_cap = new_cap;
}

/* Test if an integer n is a triangular number. */
static inline bool is_triangular(seq_t n)
{
    seq_t t = (seq_t)8 * n + 1;
    seq_t s = floor_sqrt(t);
    return s * s == t;
}

/* Generate all triples for a given z and append them to the term list. */
static void generate_for_z(idx_t z)
{
    seq_t nTri = (seq_t)z * ((seq_t)z + 1) / 2;   /* T(z) */
    seq_t sqrt_nTri = floor_sqrt(nTri);

    for (seq_t d = 1; d <= sqrt_nTri; ++d) {
        if (nTri % d != 0) continue;             /* not a divisor */
        seq_t e = nTri / d;                      /* complementary factor */

        if (!is_triangular(d) || !is_triangular(e)) continue;

        /* Compute x and y from the triangular numbers d and e. */
        seq_t t_d = (seq_t)8 * d + 1;
        seq_t s_d = floor_sqrt(t_d);
        seq_t x = (s_d - 1) / 2;

        seq_t t_e = (seq_t)8 * e + 1;
        seq_t s_e = floor_sqrt(t_e);
        seq_t y = (s_e - 1) / 2;

        if (x < 2) continue;                    /* restrict to x ≥ 2 */

        /* x ≤ y automatically holds because e ≥ d. */
        grow_terms(terms_size + 3);
        terms[terms_size++] = x;
        terms[terms_size++] = y;
        terms[terms_size++] = z;
    }
}

/* Ensure that at least `n` flattened elements are available. */
static void ensure_terms(idx_t n)
{
    if (n <= (idx_t)terms_size) return;
    while ((idx_t)terms_size < n) {
        generate_for_z(next_z);
        ++next_z;
    }
}

/* Public entry point for the OEIS sequence. */
seq_t a(idx_t n)
{
    ensure_terms(n);
    return terms[(size_t)(n - 1)];   /* 1‑based to 0‑based index */
}
