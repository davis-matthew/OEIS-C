/*
 * A352733  Least number k such that the total number of parts in all
 *          partitions of k into consecutive parts is a record.
 * https://oeis.org/A352733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A352733:  least k whose total number of parts in all consecutive-part partitions
 * is a record.  For a given k, let
 *   f(k) = Σ_{r≥1, r(r+1)/2 ≤ k, k≡0 (mod r) if r odd,
 *                k≡r/2 (mod r) if r even} r
 * then a(n) is the smallest k with f(k) larger than all previous f(j).  */
/*  The computation is done by sieving all f(k) up to a bound that is
 *  doubled until the required number of records is obtained. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Global data for the sieve and record list. */
static seq_t *farr = NULL;          /* f(k) values */
static idx_t fmax = 0;              /* current size of farr (max k computed) */
static idx_t *recarr = NULL;        /* list of record k's (1‑based) */
static idx_t reccnt = 0;            /* number of records stored */

/* Re‑compute f(k) and record list up to fmax. */
static void recompute_up_to(idx_t bound)
{
    /* free previous data */
    if (farr) free(farr);
    if (recarr) free(recarr);

    fmax = bound;
    farr = (seq_t *)malloc((bound + 1U) * sizeof(seq_t));
    if (!farr) abort();

    /* zero the array */
    for (idx_t i = 0; i <= bound; ++i) farr[i] = 0;

    /* Sieve: for each r add r to every k that is a valid partition length. */
    for (idx_t r = 1; ; ++r) {
        idx_t k0 = r * (r + 1U) / 2U;  /* smallest k with this r */
        if (k0 > bound) break;
        for (idx_t k = k0; k <= bound; k += r) {
            farr[k] += (seq_t)r;
        }
    }

    /* Collect record positions */
    recarr = (idx_t *)malloc((bound + 1U) * sizeof(idx_t));
    if (!recarr) abort();
    reccnt = 0;
    seq_t max_f = 0;
    for (idx_t k = 1; k <= bound; ++k) {
        if (farr[k] > max_f) {
            max_f = farr[k];
            recarr[reccnt++] = k;
        }
    }
}

/* Ensure that we have at least need_n records computed.  */
static void ensure_records(idx_t need_n)
{
    if (reccnt >= need_n) return;

    /* Start with a modest bound and double until enough records. */
    if (fmax == 0) {
        fmax = 100000U;    /* initial guess */
    }

    while (reccnt < need_n) {
        recompute_up_to(fmax);
        if (reccnt >= need_n) break;
        /* increase bound aggressively */
        fmax = fmax * 2U + 1000U;
    }
}

/* Required function definition. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* out of range, but keep harmless */
    ensure_records(n);
    return (seq_t)recarr[n - 1U];
}
