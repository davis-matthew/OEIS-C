/*
 * A354715  Numbers k such that the number of divisors of k divides k-2.
 * https://oeis.org/A354715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 9141, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A354715: numbers k such that the number of divisors of k divides k-2.
   The function returns the n‑th such number (a(1) = 1).  It is implemented by
   incrementally sieving divisor counts up to the largest candidate required
   and storing the results in a static array so that future calls are fast. */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

/* Static data used for memoization */
static seq_t *divcnt = NULL;       /* divisor counts, index 0 unused */
static idx_t   max_limit = 0;      /* current maximum k for which divcnt is valid */

static seq_t *terms = NULL;        /* list of found sequence terms */
static size_t terms_capacity = 0;
static size_t terms_count = 0;
static idx_t  next_k_checked = 1;  /* next integer to test for membership */

/* Recompute divisor counts up to 'need', discarding any previous values.
   The array 'divcnt' is grown to accommodate all numbers up to need. */
static void ensure_divcnt(idx_t need)
{
    if (need <= max_limit) return;

    idx_t new_limit = (max_limit == 0) ? 128 : max_limit;
    while (new_limit < need) new_limit <<= 1;   /* double until sufficient */

    seq_t *tmp = (seq_t *)realloc(divcnt, (new_limit + 1) * sizeof(seq_t));
    if (!tmp) abort();                          /* out of memory */
    divcnt = tmp;
    /* Zero all entries (including those already present, simpler and safe). */
    memset(divcnt, 0, (new_limit + 1) * sizeof(seq_t));

    /* Simple divisor sieve: for each i increase all multiples. */
    for (idx_t i = 1; i <= new_limit; ++i) {
        for (idx_t j = i; j <= new_limit; j += i)
            divcnt[j]++;                         /* count divisor i of j */
    }

    max_limit = new_limit;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n < 1 */

    /* Keep generating terms until we have at least n of them. */
    while (terms_count < n) {
        /* Expand divisor count array if needed. */
        if (next_k_checked > max_limit)
            ensure_divcnt(next_k_checked);

        seq_t d = divcnt[next_k_checked];
        bool ok = false;

        if (next_k_checked == 1) {
            ok = true;                            /* 1 always satisfies (-1 % 1 == 0) */
        } else {
            seq_t diff = ((seq_t)next_k_checked) - 2;
            if (diff % d == 0)
                ok = true;
        }

        if (ok) {
            /* Grow the terms array if necessary. */
            if (terms_capacity == 0) {
                terms_capacity = 128;
                terms = (seq_t *)malloc(sizeof(seq_t) * terms_capacity);
                if (!terms) abort();
            } else if (terms_count == terms_capacity) {
                terms_capacity <<= 1;
                seq_t *tmp = (seq_t *)realloc(terms, sizeof(seq_t) * terms_capacity);
                if (!tmp) abort();
                terms = tmp;
            }
            terms[terms_count++] = (seq_t)next_k_checked;
        }

        ++next_k_checked;
    }

    return terms[n - 1];
}
