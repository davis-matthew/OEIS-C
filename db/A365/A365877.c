/*
 * A365877  a(n) is the number of quadratic equations u*x^2 + v*x + w = 0 with
 *          distinct solution sets L != {} and integer coefficients u, v, w,
 *          where n >= abs(u) + abs(v) + abs(w) and the sum of the solutions
 *          equals the product of the solutions.
 * https://oeis.org/A365877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ (u , v) : u>0 , gcd(u,v)=1 , v(v+4*u)>=0 , u+2|v| <= n
 *            }|
 * Range:     u8 n <= 38, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) counts primitive integer triples (u , v , w) with w = -v and
   non‑negative discriminant, where the absolute sum |u|+|v|+|w| does not
   exceed n.  The value returned for each n is the cumulative count
   up to n. */

#include <stdlib.h>
#include <stdbool.h>

/* ---------- Helper routines ---------- */

/* Greatest common divisor for idx_t values (unsigned 64‑bit).  */
static inline idx_t gcd_idx_t(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* ---------- Caching of prefix counts ---------- */

static seq_t *freq = NULL;        /* freq[s] = number of triples with sum s   */
static seq_t *cum  = NULL;        /* cum[n]  = a(n)                          */
static idx_t   max_limit = 0;     /* maximum n for which arrays are valid    */

/* Recompute the entire table up to the requested limit.  */
static void recompute_up_to(idx_t limit)
{
    /* Allocate zeroed arrays.  */
    seq_t *new_freq = (seq_t *)calloc((size_t)(limit + 1), sizeof(seq_t));
    if (!new_freq) return;   /* allocation failure – unlikely for limits <= 10000 */

    seq_t *new_cum = (seq_t *)calloc((size_t)(limit + 1), sizeof(seq_t));
    if (!new_cum) {
        free(new_freq);
        return;
    }

    /* Count all primitive triples (u , v , w=-v) with abs sum <= limit. */
    for (idx_t u = 1; u <= limit; ++u) {
        idx_t max_v = (limit - u) / 2;   /* because u + 2|v| <= limit */

        /* Non‑negative v (including 0).  */
        for (idx_t v = 0; v <= max_v; ++v) {
            if (v == 0) {
                if (u == 1)  /* gcd(1,0) == 1, this is the only valid case */
                    new_freq[1] += 1;
                continue;
            }
            if (gcd_idx_t(u, v) == 1) {
                idx_t s = u + 2 * v;
                new_freq[s] += 1;
            }
        }

        /* Negative v: let t = |v| >= 4*u to satisfy discriminant ≥ 0. */
        idx_t start_t = 4 * u;
        if (start_t <= max_v) {
            for (idx_t t = start_t; t <= max_v; ++t) {
                if (gcd_idx_t(u, t) == 1) {
                    idx_t s = u + 2 * t;
                    new_freq[s] += 1;
                }
            }
        }
    }

    /* Build cumulative counts from frequency array. */
    new_cum[0] = 0;
    for (idx_t i = 1; i <= limit; ++i) {
        new_cum[i] = new_cum[i - 1] + new_freq[i];
    }

    /* Replace old tables with the new ones. */
    free(freq);
    free(cum);
    freq = new_freq;
    cum  = new_cum;
    max_limit = limit;
}

/* ---------- Public sequence routine ---------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n > max_limit)
        recompute_up_to(n);
    return cum[n];
}
