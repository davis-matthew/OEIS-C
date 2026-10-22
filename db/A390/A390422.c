/*
 * A390422  Norms of irreducible elements of Z[(1+sqrt(-15))/2], the ring of
 *          integers of Q(sqrt(-15)).
 * https://oeis.org/A390422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 3994, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Norms of irreducible elements of Z[(1+sqrt(-15))/2] – OEIS A390422
 *
 * A(n) is the n‑th norm of an irreducible element in the ring of integers of Q(sqrt(-15)).
 * The norm of a + bω (ω = (1+sqrt(-15))/2) is N = a^2 + a*b + 4*b^2.
 * An integer is a norm of some ring element iff it can be expressed as
 * a^2 + a*b + 4*b^2 for integers a,b.  It is the norm of an irreducible
 * element precisely when it is representable and cannot be factored into two
 * smaller representables (>1).  This function generates enough representables,
 * discards the reducible ones, and returns the n‑th remaining norm.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* The build system supplies seq_t and idx_t; do not define them. */

/* Dynamic cache of irreducible norms */
static seq_t *irred_vals = NULL;   /* array of irreducible norms */
static size_t irred_count = 0;      /* number of stored norms */
static idx_t cur_limit = 0;         /* highest integer examined */

/* Rebuild the cache until it contains at least `needed` norms. */
static void rebuild_if_needed(idx_t needed)
{
    if (irred_count >= needed)
        return;

    size_t limit = cur_limit ? (size_t)cur_limit * 2 : 1000;
    if (limit < 1000)
        limit = 1000;

    while (1) {
        /* Allocate arrays for representability and reducibility flags. */
        uint8_t *is_rep = (uint8_t*)calloc(limit + 1, sizeof(uint8_t));
        if (!is_rep) exit(EXIT_FAILURE);
        uint8_t *is_red = (uint8_t*)calloc(limit + 1, sizeof(uint8_t));
        if (!is_red) { free(is_rep); exit(EXIT_FAILURE); }

        /* Mark numbers that are representable as a^2 + a*b + 4*b^2. */
        int maxA = (int)(sqrt((double)limit) + 2);
        int maxB = maxA;
        for (int a = -maxA; a <= maxA; ++a) {
            for (int b = -maxB; b <= maxB; ++b) {
                long long val = (long long)a * a + (long long)a * b + 4LL * b * b;
                if (val >= 2 && val <= (long long)limit)
                    is_rep[(size_t)val] = 1;
            }
        }

        /* Build a list of representable numbers. */
        size_t rep_cap = limit / 4 + 10;
        uint32_t *rep_list = (uint32_t*)malloc(rep_cap * sizeof(uint32_t));
        if (!rep_list) { free(is_rep); free(is_red); exit(EXIT_FAILURE); }
        size_t rep_cnt = 0;
        for (size_t n = 2; n <= limit; ++n) {
            if (is_rep[n]) {
                if (rep_cnt >= rep_cap) {
                    rep_cap *= 2;
                    rep_list = (uint32_t*)realloc(rep_list, rep_cap * sizeof(uint32_t));
                    if (!rep_list) { free(is_rep); free(is_red); exit(EXIT_FAILURE); }
                }
                rep_list[rep_cnt++] = (uint32_t)n;
            }
        }

        /* Mark reducible numbers as products of two representables. */
        for (size_t i = 0; i < rep_cnt; ++i) {
            uint32_t u = rep_list[i];
            for (size_t j = i; j < rep_cnt; ++j) {
                uint32_t v = rep_list[j];
                unsigned long long prod = (unsigned long long)u * v;
                if (prod > (unsigned long long)limit)
                    break;
                is_red[(size_t)prod] = 1;
            }
        }

        /* Collect irreducible norms. */
        size_t irr_cap = rep_cnt;   /* upper bound */
        seq_t *irr_tmp = (seq_t*)malloc(irr_cap * sizeof(seq_t));
        if (!irr_tmp) { free(is_rep); free(is_red); free(rep_list); exit(EXIT_FAILURE); }
        size_t irr_cnt = 0;
        for (size_t n = 2; n <= limit; ++n) {
            if (is_rep[n] && !is_red[n]) {
                if (irr_cnt >= irr_cap) {
                    irr_cap *= 2;
                    irr_tmp = (seq_t*)realloc(irr_tmp, irr_cap * sizeof(seq_t));
                    if (!irr_tmp) { free(is_rep); free(is_red); free(rep_list); exit(EXIT_FAILURE); }
                }
                irr_tmp[irr_cnt++] = (seq_t)n;
            }
        }

        free(is_rep);
        free(is_red);
        free(rep_list);

        if (irr_cnt >= needed) {
            /* Replace old cache. */
            free(irred_vals);
            irred_vals = irr_tmp;
            irred_count = irr_cnt;
            cur_limit = (idx_t)limit;
            return;
        }

        /* Not enough: discard temporary data and try a larger limit. */
        free(irr_tmp);
        limit = limit * 2;
        if (limit == 0) /* overflow guard */
            limit = SIZE_MAX;
    }
}

/* Public function requested by the OEIS framework. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n=0 – return 0 safely */
    rebuild_if_needed(n);
    return irred_vals[n - 1];
}
