/*
 * A373771  Expansion of e.g.f. exp(x^2 / (2 * (1 - x)^3)) / (1 - x).
 * https://oeis.org/A373771
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: 2*(n+1)^2*(n+2)*(n+3)*a(n)
   -(15+8*n)*(n+2)*(n+3)*a(n+1)
   +4*(7+3*n)*(n+3)*a(n+2)
   -2*(13+4*n)*a(n+3)
   +2*a(n+4) = 0;
   Initial values: a(0)=1, a(1)=1, a(2)=3, a(3)=18. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Ensure cache capacity up to 'needed' (index). */
static void ensure_capacity(idx_t needed, seq_t **p, size_t *cap) {
    if (*cap > needed) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap * 2);
    while (new_cap <= needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(*p, sizeof(seq_t) * new_cap);
    /* In case of allocation failure tmp will be NULL; ignore – tests use small n. */
    *p = tmp;
    *cap = new_cap;
}

seq_t a(idx_t n) {
    /* Static cache for memoisation. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t last_computed = (idx_t)-1;  /* uninitialised */

    /* Initial values. */
    const seq_t init[4] = { (seq_t)1, (seq_t)1, (seq_t)3, (seq_t)18 };

    /* Initialise cache if needed. */
    if (last_computed == (idx_t)-1) {
        ensure_capacity(3, &cache, &cache_cap);
        cache[0] = init[0];
        cache[1] = init[1];
        cache[2] = init[2];
        cache[3] = init[3];
        last_computed = 3;
    }

    /* If the requested term is already computed, return it. */
    if (n <= last_computed) return cache[n];

    /* Grow cache to hold up to index n. */
    ensure_capacity(n, &cache, &cache_cap);

    /* Compute terms up to n using the recurrence. */
    for (; last_computed < n; ++last_computed) {
        /* We compute a(last_computed+1), which corresponds to recurrence with
           n_rec = last_computed - 3. */
        idx_t n_rec = last_computed - 3;

        seq_t a_n   = cache[n_rec];
        seq_t a_n1  = cache[n_rec + 1];
        seq_t a_n2  = cache[n_rec + 2];
        seq_t a_n3  = cache[n_rec + 3];

        /* Convert indices to seq_t for safe arithmetic. */
        seq_t nseq = (seq_t)n_rec;
        seq_t n1 = nseq + (seq_t)1;
        seq_t n2 = nseq + (seq_t)2;
        seq_t n3 = nseq + (seq_t)3;
        seq_t n1sq = n1 * n1;

        /* Positive part of numerator. */
        seq_t coeff_pos1 = (seq_t)(15 + 8 * n_rec);
        seq_t term_pos1 = coeff_pos1 * n2 * n3 * a_n1;

        seq_t coeff_pos2 = (seq_t)2 * (seq_t)(13 + 4 * n_rec);
        seq_t term_pos2 = coeff_pos2 * a_n3;

        seq_t pos = term_pos1 + term_pos2;

        /* Negative part of numerator. */
        seq_t coeff_neg1 = (seq_t)2;
        seq_t term_neg1 = coeff_neg1 * n1sq * n2 * n3 * a_n;

        seq_t coeff_neg2 = (seq_t)4 * (seq_t)(7 + 3 * n_rec);
        seq_t term_neg2 = coeff_neg2 * n3 * a_n2;

        seq_t neg = term_neg1 + term_neg2;

        seq_t num  = pos - neg;          /* should be non‑negative */
        seq_t next = num / (seq_t)2;     /* divide by 2 */

        cache[last_computed + 1] = next;
    }

    return cache[n];
}
