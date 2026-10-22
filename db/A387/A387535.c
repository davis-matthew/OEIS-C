/*
 * A387535  a(n) = maximum k such that B(n) < B(n+1) < ... < B(n+k-1), where B
 *          = bigomega (A001222).
 * https://oeis.org/A387535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = maximum length k such that bigomega(n) < bigomega(n+1) <
 *            ... < bigomega(n+k-1)
 * Range:     u8 n <= 9572, u16 n <= 9572, u32 n <= 9572, u64 n <= 9572,
 *            u128 n <= 9572, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The bigomega function is provided by the build.  Include its header. */
#include <stdlib.h>
#include "../utils/bigomega.h"

 /* Cached bigomega values.  omega[i] holds bigomega(i).  Index i is 1‑based. */
static seq_t *omega = NULL;          /* array of cached values   */
static idx_t  omega_len = 1;         /* next index that must be filled (i.e. first unused index) */
static size_t omega_cap = 0;         /* current capacity of the array */

/* Ensure the cache contains values up to and including 'up_to'. */
static void
ensure_cache(idx_t up_to)
{
    if (omega_cap == 0) {
        omega_cap = 64;
        omega = (seq_t *)malloc(omega_cap * sizeof(seq_t));
    }
    while (omega_len <= up_to) {
        if (omega_len >= omega_cap) {
            omega_cap *= 2;
            omega = (seq_t *)realloc(omega, omega_cap * sizeof(seq_t));
        }
        omega[omega_len] = bigomega((idx_t)omega_len);
        omega_len++;
    }
}

seq_t
a(idx_t n)
{
    /* n is 1‑based.  Compute the length of the longest strictly increasing
       run of bigomega values starting at n. */
    if (n == 0) return 0;          /* safety, though n>=1 in normal use */

    ensure_cache(n);
    seq_t len  = (seq_t)1;        /* at least the element at position n */
    seq_t prev = omega[n];        /* bigomega(n) */

    idx_t i = n;
    while (1) {
        idx_t nexti = i + 1;
        ensure_cache(nexti);
        seq_t cur = omega[nexti];
        if (cur > prev) {
            len++;
            i = nexti;
            prev = cur;
        } else {
            break;
        }
    }
    return len;
}
