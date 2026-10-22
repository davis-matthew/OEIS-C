/*
 * A322809  Lexicographically earliest such sequence a that a(i) = a(j) =>
 *          f(i) = f(j) for all i, j, where f(n) = -1 if n is an odd prime,
 *          and f(n) = floor(n/2) for all other numbers.
 * https://oeis.org/A322809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 507, u16 n <= 9443, u32 n <= 9443, u64 n <= 9443,
 *            u128 n <= 9443, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A322809: Lexicographically earliest sequence a such that
   a(i) = a(j) ⇔ f(i) = f(j), where
        f(n) = -1          if n is an odd prime,
        f(n) = floor(n/2)  otherwise.
   The sequence is obtained by assigning labels to distinct values of f
   as they appear: the first distinct f-value gets label 1, the next
   distinct f-value gets label 2, and so on.
   (This is the same as A323161(n+1) – 1.) */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/is_prime.h"

static seq_t *a_cache = NULL;      /* cached values of a(n) */
static idx_t   cache_len = 0;     /* number of cached terms */

static int *label_for_f = NULL;    /* label for each f-index:
                                    f = -1  → index 0
                                    f = k   → index k+1   (k = floor(n/2)) */
static size_t label_cache_len = 0;
static int next_label = 1;        /* next unused label (start at 1) */

/* Ensure that all terms up to and including n are computed and cached. */
static void ensure_upto(idx_t n)
{
    if (n <= cache_len)
        return;   /* already computed */

    /* Grow the a(n) cache to size n. */
    a_cache = (seq_t *)realloc(a_cache, n * sizeof(seq_t));
    if (!a_cache)            /* allocation failed – abort further work */
        return;
    /* Compute the new terms from (cache_len + 1) to n. */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        /* ---- Determine the index into label_for_f ---- */
        size_t fidx;
        if ((i & 1ULL) && is_prime(i)) {
            fidx = 0;                     /* f(i) = -1 → index 0 */
        } else {
            uint64_t k = i >> 1;          /* floor(i/2) */
            fidx = (size_t)k + 1;         /* map k → index k+1 */
        }

        /* Grow the label cache if necessary. */
        if (fidx >= label_cache_len) {
            size_t new_len = fidx + 1;
            int *tmp = (int *)realloc(label_for_f, new_len * sizeof(int));
            if (!tmp)               /* allocation failed – abort */
                return;
            /* Initialise the new entries to 0. */
            memset(tmp + label_cache_len, 0,
                   (new_len - label_cache_len) * sizeof(int));
            label_for_f    = tmp;
            label_cache_len = new_len;
        }

        /* ---- Fetch or assign the label for this f-value ---- */
        int lab = label_for_f[fidx];
        if (lab == 0) {
            lab = next_label++;
            label_for_f[fidx] = lab;
        }

        a_cache[i - 1] = (seq_t)lab; /* store the computed value */
    }
    cache_len = n;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* undefined for n < 1; guard against misuse */
    ensure_upto(n);
    return a_cache[n - 1];
}
