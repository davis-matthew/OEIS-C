/*
 * A364662  Numbers k such that sigma(k) < sigma(k+1) < sigma(k+2) <
 *          sigma(k+3).
 * https://oeis.org/A364662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 448, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is the set of integers k such that
   σ(k) < σ(k+1) < σ(k+2) < σ(k+3).
   Here the function builds the list incrementally by testing
   successive k until the requested term is found. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/sigma_of.h"   /* provides seq_t sigma_of(idx_t n) */

static idx_t *good_k   = NULL;   /* dynamic array of qualifying k's   */
static size_t good_cnt = 0;      /* number stored                 */
static size_t good_cap = 0;      /* current capacity               */
static idx_t next_k    = 1;      /* next k to test                 */

/* Grow the good_k array to at least 'min_cap' elements. */
static void grow_good(size_t min_cap)
{
    if (good_cap < min_cap) {
        size_t new_cap = (good_cap == 0) ? 16 : good_cap * 2;
        while (new_cap < min_cap) new_cap *= 2;
        idx_t *tmp = (idx_t *)realloc(good_k, new_cap * sizeof(idx_t));
        if (!tmp) abort();            /* allocation failure */
        good_k   = tmp;
        good_cap = new_cap;
    }
}

/* Ensure that at least 'need' qualifying k's are stored in good_k. */
static void ensure_good(idx_t need)
{
    if (good_cnt >= need) return;

    while (good_cnt < need)
    {
        idx_t k = next_k;          /* current k to test        */
        seq_t s0 = sigma_of(k);
        seq_t s1 = sigma_of(k + 1);
        if (s0 < s1) {
            seq_t s2 = sigma_of(k + 2);
            if (s1 < s2) {
                seq_t s3 = sigma_of(k + 3);
                if (s2 < s3) {
                    /* store the qualifying k */
                    grow_good(good_cnt + 1);
                    good_k[good_cnt++] = k;
                }
            }
        }
        ++next_k;
    }
}

/* --- required public function ----------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, but safe  */

    ensure_good(n);                       /* make sure we have n terms */
    return (seq_t)good_k[n - 1];
}
