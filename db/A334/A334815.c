/*
 * A334815  Unitary arithmetic numbers k (A103826) such that usigma(k)/ud(k)
 *          is also a unitary arithmetic number, where ud(k) is the number of
 *          divisors of k (A034444) and usigma(k) is their sum (A034448).
 * https://oeis.org/A334815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 150, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   unitary_sigma_and_divisor_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A334815 – Unitary arithmetic numbers k such that usigma(k)/ud(k)
    is also a unitary arithmetic number.
    a(n) is the nth term in increasing order; a(1)=1.
    The function builds the sequence on demand and memoises the
    already found terms.  It works for any seq_t and idx_t
    (uint64_t for idx_t).  For arbitrary‑precision seq_t a cast
    from void* to seq_t* is required when realloc/malloc are used.  */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/unitary_sigma_and_divisor_count.h"

/* forward declaration of helper from utils if available */
void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt);

/* Dynamic array of terms  */
static seq_t *terms = NULL;
static size_t terms_len = 0;         /* number of stored terms */
static size_t terms_cap = 0;         /* current capacity of terms[] */
static uint64_t checked_upto = 0;    /* last integer processed      */

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/*  add_term – append a value to the dynamic array of terms.           */
static void add_term(seq_t val)
{
    if (terms_len == terms_cap) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 16;
        seq_t *new_arr = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
        if (!new_arr) exit(EXIT_FAILURE);
        terms = new_arr;
        terms_cap = new_cap;
    }
    terms[terms_len++] = val;
}

/* ------------------------------------------------------------------ */
/*  generate_upto – generate terms until at least `target` terms are
    stored.  */
static void generate_upto(size_t target)
{
    while (terms_len < target) {
        ++checked_upto;
        uint64_t k = checked_upto;

        /* k must be a unitary arithmetic number */
        seq_t ud_k, usig_k;
        idx_t cnt_k;
        unitary_sigma_and_divisor_count(k, &ud_k, &usig_k, &cnt_k);
        if (!((int)cnt_k >= 0)) continue;            /* impossible, keep compiler happy */

        if (usig_k % ud_k != 0) continue;   /* not unitary arithmetic */
        seq_t r = usig_k / ud_k;            /* integer ratio */

        /* r must also be a unitary arithmetic number */
        seq_t ud_r, usig_r;
        idx_t cnt_r;
        if ((uint64_t)r > ULLONG_MAX) continue;      /* cannot handle larger values */
        unitary_sigma_and_divisor_count((uint64_t)r, &ud_r, &usig_r, &cnt_r);
        if (usig_r % ud_r == 0) {
            add_term((seq_t)k);
        }
    }
}

/* ------------------------------------------------------------------ */
/*  public function a(n) – returns the nth term of A334815.            */
seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* undefined, but safe guard */

    /* initialise on first call */
    if (!terms) {
        terms_cap = 16;
        terms     = (seq_t*)malloc(terms_cap * sizeof(seq_t));
        if (!terms) exit(EXIT_FAILURE);
        terms_len   = 1;
        terms[0]    = 1;       /* k = 1 always satisfies the condition */
        checked_upto = 1;
    }

    generate_upto((size_t)n);          /* ensure at least n terms */
    return terms[n - 1];
}
