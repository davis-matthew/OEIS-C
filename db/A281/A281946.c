/*
 * A281946  Number of sequences of balls colored with at most n colors such
 *          that exactly two balls are of a color seen earlier in the
 *          sequence.
 * https://oeis.org/A281946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 30,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 100 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A281946
   Formula: a(n) = n! * Sum_{k=3..n+2} (C(k,3)+3*C(k,4))/(n+2-k)! */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <math.h>
#include "../utils/mem.h"

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

static void seq_free_array(seq_t* p)          { mem_free(p); }

static inline seq_t h_coeff(idx_t k)
{
    /* h(k) = C(k,3) + 3*C(k,4), computed with 128‑bit intermediates */
    unsigned __int128 kk = k;
    unsigned __int128 c3 = kk * (kk - 1) * (kk - 2) / 6;
    unsigned __int128 c4 = kk * (kk - 1) * (kk - 2) * (kk - 3) / 24;
    unsigned __int128 h  = c3 + 3 * c4;
    return (seq_t)h;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* not used; a(1) is first term */

    /* ---- factorial cache ---- */
    static seq_t *fact = NULL;
    static idx_t fact_len = 0;

    if (fact == NULL) {
        fact = seq_alloc_new_array(1);   /* fact[0] */
        fact[0] = (seq_t)1;
        fact_len = 1;
    }

    if (fact_len <= n) {
        /* extend factorial array to size n+1 */
        seq_t *newfact = seq_alloc_new_array(n + 1);
        for (idx_t i = 0; i < fact_len; ++i) newfact[i] = fact[i];
        for (idx_t i = fact_len; i <= n; ++i)
            newfact[i] = newfact[i - 1] * (idx_t)i;
        seq_free_array(fact);
        fact = newfact;
        fact_len = n + 1;
    }

    /* ---- compute a(n) ---- */
    seq_t sum   = 0;
    seq_t ratio = fact[n];                 /* n! */

    for (idx_t r = 0; r < n; ++r) {
        idx_t k = n + 2 - r;               /* k ranges from n+2 down to 3 */
        seq_t coeff = h_coeff(k);          /* C(k,3)+3*C(k,4)  */
        sum += ratio * coeff;
        ratio /= (r + 1);                  /* prepare ratio for next loop */
    }

    return sum;
}
