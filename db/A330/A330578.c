/*
 * A330578  a(n) is the remainder when the sum of the first n composite
 *          numbers is divided by the n-th composite number.
 * https://oeis.org/A330578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (sum of first n composite numbers) mod (n-th composite
 *            number)
 * Range:     u8 n <= 16, u16 n <= 506, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* bool is_prime(idx_t) */

/* static state for memoisation */
static idx_t *comps = NULL;         /* 1‑based array of composite numbers       */
static seq_t *cumul = NULL;         /* 1‑based cumulative sums (cumul[0] = 0)   */
static size_t comp_cap = 0;         /* allocated capacity (excluding index 0)   */
static idx_t comp_cnt = 0;          /* number of composites stored so far      */
static idx_t next_candidate = 4;    /* next integer to test for compositeness  */

/* ensure allocation for at least 'needed' composite values (1‑based) */
static void ensure_comp_cap(size_t needed)
{
    if (comp_cap < needed) {
        size_t new_cap = comp_cap ? comp_cap * 2 : 128;
        if (new_cap < needed) new_cap = needed;

        comps  = (idx_t*)realloc(comps,  sizeof(idx_t)  * (new_cap + 1));
        cumul  = (seq_t*)realloc(cumul,  sizeof(seq_t)  * (new_cap + 1));
        comp_cap = new_cap;

        /* initialise the first cumulative entry (used for comp_cnt==0) */
        if (comp_cnt == 0) {
            cumul[0] = 0;
        }
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* not part of the sequence, safe fallback */

    /* build composites and cumulative sums up to n */
    ensure_comp_cap((size_t)n);

    while (comp_cnt < n) {
        /* find next composite number */
        while (is_prime(next_candidate)) ++next_candidate;

        ++comp_cnt;
        comps[comp_cnt] = next_candidate;
        cumul[comp_cnt] = cumul[comp_cnt - 1] + comps[comp_cnt];
        ++next_candidate;
    }

    /* compute a(n) = sum_{k=1..n} comp[k] mod comp[n] */
    return cumul[n] % comps[n];
}
