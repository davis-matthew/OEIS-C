/*
 * A286610  Restricted growth sequence computed for Euler totient function
 *          phi, A000010.
 * https://oeis.org/A286610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 960, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A286610 – restricted growth sequence for Euler totient φ.
   For φ(n) first encountered, assign the smallest unused natural number.
   Implements an incremental sieve for φ and memoises a(1..N). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Static storage for memoisation */
static seq_t *seq_arr = NULL;          /* a[0..N-1] */
static size_t seq_len = 0;             /* largest N computed so far */

static uint64_t *phi_tbl = NULL;      /* φ[1..N] as uint64_t */
static size_t phi_cap = 0;             /* capacity of phi_tbl (+1) */

static uint64_t *label_tbl = NULL;     /* label for each φ value (0..N) */
static size_t lbl_cap = 0;             /* capacity of label_tbl (+1) */

static uint64_t next_label = 1;        /* next unused label */

/* ------------------------------------------------------------------ */
/* Helper: Resize and fill tables up to (and including) n.        */
/* ------------------------------------------------------------------ */
static void ensure_upto(idx_t n)
{
    if (n <= seq_len)                 /* already computed */
        return;

    /* Resize phi table if needed */
    size_t new_cap = (size_t)n + 1;   /* indices 0..n */
    if (phi_cap < new_cap) {
        uint64_t *tmp = (uint64_t *)realloc(phi_tbl, new_cap * sizeof(uint64_t));
        if (!tmp) exit(EXIT_FAILURE);
        phi_tbl = tmp;

        /* initialise and sieve φ for 1..n */
        for (size_t i = 1; i < new_cap; ++i)
            phi_tbl[i] = (uint64_t)i;
        for (size_t p = 2; p < new_cap; ++p) {
            if (phi_tbl[p] == (uint64_t)p) {      /* prime */
                for (size_t j = p; j < new_cap; j += p)
                    phi_tbl[j] -= phi_tbl[j] / (uint64_t)p;
            }
        }
        phi_cap = new_cap;
    }

    /* Resize label table if needed */
    if (lbl_cap < new_cap) {
        uint64_t *tmp = (uint64_t *)realloc(label_tbl, new_cap * sizeof(uint64_t));
        if (!tmp) exit(EXIT_FAILURE);
        label_tbl = tmp;
        for (size_t i = lbl_cap; i < new_cap; ++i)
            label_tbl[i] = (uint64_t)0;
        lbl_cap = new_cap;
    }

    /* Resize sequence array if needed */
    if (seq_len < (size_t)n) {
        seq_t *tmp = (seq_t *)realloc(seq_arr, n * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        seq_arr = tmp;              /* contents beyond seq_len are uninitialised */
    }

    /* Compute a(i) for i = seq_len+1 .. n */
    for (size_t i = seq_len + 1; i <= (size_t)n; ++i) {
        uint64_t phi_i = phi_tbl[i];
        uint64_t lbl = label_tbl[phi_i];
        if (lbl == (uint64_t)0) {
            lbl = next_label++;
            label_tbl[phi_i] = lbl;
        }
        seq_arr[i - 1] = (seq_t)lbl;
    }

    seq_len = (size_t)n;
}

/* Public function required by the build system */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* defined behaviour for n < 1 */
    ensure_upto(n);
    return seq_arr[(size_t)n - 1];
}
