/*
 * A253396  Number of (n+1)X(7+1) 0..1 arrays with every 2X2 subblock
 *          antidiagonal maximum minus diagonal minimum nondecreasing
 *          horizontally and diagonal maximum minus antidiagonal minimum
 *          nondecreasing vertically.
 * https://oeis.org/A253396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 19, u32 n <= 210, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A253396:
   a(n) = 4*a(n-1) - 5*a(n-2) + 5*a(n-4) - 4*a(n-5) + a(n-6)
   (for n > 18, with the given initial values) */

#include <stdlib.h>   /* malloc, realloc, free */

static const seq_t init_vals[] = {
    0,          /* unused 0th entry */
    704, 391, 520, 823, 1269, 1855, 2726, 3810, 5311, 7163,
    9569, 12493, 16140, 20493, 25773, 31978, 39346, 47891,
    57867, 69304, 82472, 97417, 114425, 133558
}; /* init_vals[1..24] */

seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* sequence is defined for n >= 1 */

    /* Persistent storage for computed terms */
    static seq_t *terms = NULL;
    static size_t terms_cap = 0;    /* capacity in elements */
    static size_t terms_len = 0;    /* highest index that has been filled */
    static int initialised = 0;     /* flag for one‑time init */

    /* Ensure enough capacity for index n (1‑based) */
    if (terms_cap <= (size_t)n) {
        size_t new_cap = (size_t)(n + 1);
        if (new_cap < 64) new_cap = 64;
        while (new_cap <= (size_t)n) new_cap <<= 1;

        seq_t *tmp = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) return 0;            /* allocation failure, safe fallback */
        terms = tmp;
        terms_cap = new_cap;
    }

    /* One‑time copying of the first 24 terms */
    if (!initialised) {
        for (idx_t i = 1; i <= 24; ++i) terms[i] = init_vals[i];
        terms_len = 24;
        initialised = 1;
    }

    /* If the desired term is already known, return it immediately */
    if ((size_t)n <= terms_len) return terms[n];

    /* Compute terms beyond the current length up to n using the recurrence */
    for (idx_t i = terms_len + 1; i <= n; ++i) {
        seq_t t1 = terms[i-1];  t1 *= (seq_t)4;
        seq_t t2 = terms[i-2];  t2 *= (seq_t)5;  t1 -= t2;
        seq_t t4 = terms[i-4];  t4 *= (seq_t)5;  t1 += t4;
        seq_t t5 = terms[i-5];  t5 *= (seq_t)4;  t1 -= t5;
        seq_t t6 = terms[i-6];                 t1 += t6;
        terms[i] = t1;
    }
    terms_len = n;
    return terms[n];
}
