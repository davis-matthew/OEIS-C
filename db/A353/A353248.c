/*
 * A353248  Irregular table, read by rows, where row n is the concatenation of
 *          all prime signatures leading to n divisors, in reverse
 *          lexicographic order.
 * https://oeis.org/A353248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A353248: prime signatures concatenated by divisor count,
   reverse lexicographic order. */

#include <stdint.h>
#include <stdbool.h>

/* Count how many terms (exponents) are produced by all signatures whose
   product of (e+1) equals 'rem'.
   'max_factor' limits the largest factor (e+1) allowed so that the
   signature is non‑increasing.
   'depth' is the number of exponents already chosen in the partial
   signature. */
static idx_t count_terms(idx_t rem, idx_t max_factor, int depth)
{
    if (rem == 1) {
        return (idx_t)depth;
    }

    idx_t total = 0;
    int start = (int)rem < (int)max_factor ? (int)rem : (int)max_factor;
    for (int f = start; f >= 2; --f) {
        if (rem % f == 0) {
            total += count_terms(rem / (idx_t)f, (idx_t)f, depth + 1);
        }
    }
    return total;
}

/* Search within the signatures of divisor count 'rem' to find the term
   at position 'off' (zero based).  The depth of the current partial
   signature is 'depth'.  The array 'cur' holds the current partial
   signature: cur[0..depth-1] are the values f = e+1 chosen so far. */
static bool dfs_find(idx_t rem, idx_t max_factor, int depth,
                     int *cur, idx_t *off, seq_t *res)
{
    if (rem == 1) {
        /* Complete signature found: it contributes 'depth' terms. */
        if (*off < depth) {
            /* The required term is the (off)th exponent of this signature. */
            *res = (seq_t)(cur[*off] - 1);
            return true;
        }
        *off -= depth;
        return false;
    }

    int start = (int)rem < (int)max_factor ? (int)rem : (int)max_factor;
    for (int f = start; f >= 2; --f) {
        if (rem % f == 0) {
            cur[depth] = f;
            if (dfs_find(rem / (idx_t)f,
                         (idx_t)f,
                         depth + 1,
                         cur, off, res))
                return true;
        }
    }
    return false;
}

/* Public function for the OEIS sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* n is 1‑based; never called with n=0 */
    idx_t cumulative = 0;

    /* Find the divisor count 'd' whose row contains the nth element. */
    for (idx_t d = 1; ; ++d) {
        idx_t row_terms = count_terms(d, d, 0);
        if (cumulative + row_terms >= n) {
            idx_t offset = n - cumulative - 1; /* zero‑based within the row */

            int cur[64];              /* depth will never exceed 64 */
            seq_t result;
            bool ok = dfs_find(d, d, 0, cur, &offset, &result);
            (void)ok;                /* should always be true */
            return result;
        }
        cumulative += row_terms;
    }
}
