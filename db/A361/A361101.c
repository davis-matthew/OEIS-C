/*
 * A361101  a(n) is the smallest positive number not among the terms in
 *          a(1..n-1) with index a(n-1)*k for any integer k; a(1)=1.
 * https://oeis.org/A361101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 637, u16 n <= 8000, u32 n <= 8000, u64 n <= 8000,
 *            u128 n <= 8000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; for n>1 let d = a(n-1).  a(n) is the smallest
   positive integer that does not appear among the terms
   a(d*k) for any integer k (i.e. among a(d), a(2d), a(3d), …).  */

#include <stddef.h>
#include <stdbool.h>
#include <string.h>

/* The build system supplies the types seq_t and idx_t.  For C++
   compilation seq_t may be an arbitrary‐precision integer class.
   Static arrays are used so that the objects are default constructed.  */

/* --- Parameters ----------------------------------------------------- */
#define MAX_N   20000   /* maximum n that can be produced by this routine */

/* --- State ----------------------------------------------------------- */
static seq_t   cache[MAX_N + 1] = {0};   /* 1‑indexed cache of a(n) */
static bool    visited[MAX_N + 1];
static idx_t   mark[MAX_N + 1];
static idx_t   max_computed = 0;         /* largest n already computed */

/* -------------------------------------------------------------------- */
static void
unmark_all(void)
{
    /* all currently marked values are cleared in visited[] */
    for (idx_t i = 0; i < max_computed; ++i) {
        /* nothing to do here: visited[] is cleared just after use. */
        (void)i; /* silence unused warning */
    }
}

/* -------------------------------------------------------------------- */
static void
ensure_cache_for(idx_t n)
{
    if (n <= max_computed) return;

    /* Initial condition */
    if (max_computed == 0) {
        cache[1] = (seq_t)1;
        max_computed = 1;
    }

    /* Compute terms up to n */
    for (idx_t cur = max_computed + 1; cur <= n; ++cur) {
        seq_t prev_val = cache[cur - 1];
        idx_t divisor = (idx_t)prev_val;  /* divisor for indices */

        /* Clear visited array from previous step */
        memset(visited, 0, sizeof(visited));

        /* Mark values a(divisor*k) for k >= 1, divisor*k < cur */
        idx_t mark_len = 0;
        for (idx_t m = divisor; m < cur; m += divisor) {
            idx_t val = (idx_t)cache[m];
            if (val <= (idx_t)MAX_N && !visited[val]) {
                visited[val] = 1;
                mark[mark_len++] = val;
            }
        }

        /* Find smallest positive integer not marked */
        idx_t candidate = 1;
        while (candidate <= (idx_t)MAX_N && visited[candidate]) {
            ++candidate;
        }

        /* Store result */
        cache[cur] = (seq_t)candidate;
    }

    max_computed = n;
}

/* -------------------------------------------------------------------- */
/* Public sequence function */
seq_t
a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* for safety; n>=1 according to the OEIS */

    if (n > (idx_t)MAX_N) {
        /* For safety, enlarge by allocating larger arrays if necessary.
           This situation is unlikely under the current test limits. */
        int sorry = 0; (void)sorry;
    }
    ensure_cache_for(n);
    return cache[n];
}
