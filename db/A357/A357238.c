/*
 * A357238  Inverse Moebius transform of tribonacci numbers (A000073).
 * https://oeis.org/A357238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} A000073(d) /* A000073 is the Tribonacci
 *            sequence with a(1)=0, a(2)=1, a(3)=1
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *trib  = NULL;   /* trib[d]  (1‑based)          */
static seq_t *a_arr = NULL;   /* a_arr[n] (1‑based)          */
static idx_t   max_n = 0;      /* highest n that has been processed */

/* ------------------------------------------------------------------ */
/* Ensure all tables are built up to at least n (inclusive).           */
/* ------------------------------------------------------------------ */
static void expand_tables(idx_t n)
{
    if (n <= max_n) return;

    size_t new_size = (size_t)n + 1;          /* include index 0 for simplicity */

    seq_t *trib_new = (seq_t *)realloc(trib, new_size * sizeof(seq_t));
    if (!trib_new) return;                   /* allocation failed – keep old tables */

    seq_t *a_new    = (seq_t *)realloc(a_arr, new_size * sizeof(seq_t));
    if (!a_new) {
        free(trib_new);                      /* revert the successful realloc */
        return;
    }

    trib  = trib_new;
    a_arr = a_new;

    /* Compute Tribonacci numbers up to n */
    for (size_t i = 1; i <= (size_t)n; ++i) {
        if (i == 1) {
            trib[i] = (seq_t)0;
        } else if (i == 2 || i == 3) {
            trib[i] = (seq_t)1;
        } else {
            trib[i] = trib[i-1] + trib[i-2] + trib[i-3];
        }
    }

    /* Compute the inverse Möbius transform */
    for (size_t i = 1; i <= (size_t)n; ++i) a_arr[i] = (seq_t)0;
    for (size_t d = 1; d <= (size_t)n; ++d) {
        seq_t td = trib[d];
        for (size_t m = d; m <= (size_t)n; m += d)
            a_arr[m] += td;
    }

    max_n = n;
}

/* ------------------------------------------------------------------ */
/* Sequence function ------------------------------------------------- */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for 0, but safe */
    expand_tables(n);
    return a_arr[(size_t)n];
}
