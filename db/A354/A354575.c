/*
 * A354575  a(1) = 1; for n > 1, a(n) is the smallest positive number that has
 *          not yet appeared that is coprime to a(n-1) and the difference a(n)
 *          - a(n-1) is distinct from all previous differences.
 * https://oeis.org/A354575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   OEIS A354575
 *   a(1) = 1
 *   for n > 1 : a(n) is the least unused positive integer
 *   coprime to a(n-1) such that the signed difference a(n)-a(n-1)
 *   has not appeared in any previous step.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* dynamic arrays for the sequence and for the signed differences */
static seq_t  *seq_vals   = NULL;   /* the values a(k)                     */
static size_t seq_cap    = 0;       /* current capacity of seq_vals        */
static size_t seq_len    = 0;       /* number of values stored so far     */

static int64_t *diff_vals = NULL;   /* signed differences a(k)-a(k-1)     */
static size_t diff_cap   = 0;       /* capacity of diff_vals              */
static size_t diff_len   = 0;       /* number of differences stored       */

/* ensure seq_vals has at least 'need' slots */
static void
ensure_seq_capacity(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap : 1;
    while (new_cap < need) new_cap <<= 1;
    seq_vals = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* ensure diff_vals has at least 'need' slots */
static void
ensure_diff_capacity(size_t need)
{
    if (need <= diff_cap) return;
    size_t new_cap = diff_cap ? diff_cap : 1;
    while (new_cap < need) new_cap <<= 1;
    diff_vals = (int64_t *)realloc(diff_vals, new_cap * sizeof(int64_t));
    diff_cap = new_cap;
}

/* linear search for a value in the used array */
static bool
used_value_exists(seq_t x)
{
    for (size_t i = 0; i < seq_len; ++i)
        if (seq_vals[i] == x) return true;
    return false;
}

/* linear search for a signed difference in diff_vals */
static bool
diff_exists(int64_t d)
{
    for (size_t i = 0; i < diff_len; ++i)
        if (diff_vals[i] == d) return true;
    return false;
}

/* compute the n-th term (n >= 1), memoising values */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined below offset, return 0 for safety */

    /* initialise static data on first call */
    if (seq_len == 0) {
        seq_cap = 4;
        seq_vals = (seq_t *)calloc(seq_cap, sizeof(seq_t));
        seq_len = 1;
        seq_vals[0] = 1;           /* a(1) = 1 */

        diff_cap = 4;
        diff_vals = (int64_t *)calloc(diff_cap, sizeof(int64_t));
        diff_len = 0;                /* no differences yet */
    }

    /* extend the sequence up to index n (1‑based) */
    while (seq_len < (size_t)n) {
        seq_t prev = seq_vals[seq_len - 1];
        seq_t cand = 1;              /* start search from 1 */

        while (true) {
            /* must be unused */
            if (used_value_exists(cand)) {
                ++cand;
                continue;
            }

            /* coprime to previous value */
            if (gcd(prev, cand) != 1) {
                ++cand;
                continue;
            }

            /* compute signed difference */
            int64_t diff;
            if (cand >= prev) {
                seq_t diff_abs = cand - prev;
                diff = (int64_t)diff_abs;                 /* positive */
            } else {
                seq_t diff_abs = prev - cand;
                diff = -(int64_t)diff_abs;               /* negative */
            }

            /* difference must be new */
            if (diff_exists(diff)) {
                ++cand;
                continue;
            }

            /* candidate accepted */
            ensure_seq_capacity(seq_len + 1);
            seq_vals[seq_len++] = cand;

            ensure_diff_capacity(diff_len + 1);
            diff_vals[diff_len++] = diff;
            break;
        }
    }

    return seq_vals[n - 1];
}
