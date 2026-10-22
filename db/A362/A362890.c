/*
 * A362890  a(1)=a(2)=1. For n>2, a(n) is the number of times that a(n-1) and
 *          a(n-2) are adjacent in the sequence thus far (in any order).
 * https://oeis.org/A362890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=a(2)=1; for n>2, a(n) = number of times that a(n-1) and
 *            a(n-2) appear as a contiguous pair (in either order) in the
 *            sequence so far
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Static cache for the generated sequence */
static seq_t *seq_cache = NULL;       /* pointer to stored terms */
static idx_t  seq_len  = 0;           /* number of terms currently stored (>=0) */
static size_t seq_cap  = 0;           /* allocated capacity of seq_cache */

/* Ensure the cache can hold at least 'need' elements */
static void ensure_capacity(idx_t need)
{
    if ((size_t)need <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 4;
    while (new_cap < (size_t)need)
        new_cap *= 2;
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cap   = new_cap;
}

/* Main function required by the build */
seq_t a(idx_t n)
{
    if (n == 0)      /* value below the offset is irrelevant */
        return 0;

    /* Extend the cache if necessary */
    if (seq_len < n) {
        ensure_capacity(n);
        for (idx_t k = seq_len + 1; k <= n; ++k) {
            if (k == 1) {                 /* a(1) */
                seq_cache[0] = 1;
            } else if (k == 2) {          /* a(2) */
                seq_cache[1] = 1;
            } else {                      /* n > 2 */
                seq_t a_nm1 = seq_cache[k - 2]; /* a(k-1) */
                seq_t a_nm2 = seq_cache[k - 3]; /* a(k-2) */
                seq_t count = 0;

                /* Count occurrences of the unordered pair (a_nm1, a_nm2) */
                for (idx_t i = 1; i <= k - 2; ++i) {
                    seq_t left  = seq_cache[i - 1];
                    seq_t right = seq_cache[i];
                    if ((left == a_nm1 && right == a_nm2) ||
                        (left == a_nm2 && right == a_nm1))
                        ++count;
                }
                seq_cache[k - 1] = count;
            }
        }
        seq_len = n;
    }
    return seq_cache[n - 1];
}
