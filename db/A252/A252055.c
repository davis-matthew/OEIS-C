/*
 * A252055  Number of products A000201(i)*A001950(j) = n.
 * https://oeis.org/A252055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = # { (i,j) | A000201(i) * A001950(j) = n } Counted by
 *            building membership tables for the two Wythoff sequences up to
 *            the queried n and inspecting all divisor pairs of n
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* Forward declaration of the table builder */
static void build_tables(idx_t up_to);

/* Static tables and current size */
static idx_t meth_size = 0;
static bool *lower_mem = NULL;
static bool *upper_mem = NULL;

/* Build boolean tables marking the lower and upper Wythoff terms
   up to 'up_to' (inclusive).  The Beatty construction
      a_k = smallest unused positive integer
      b_k = a_k + k
   produces the lower and upper sequences. */
static void build_tables(idx_t up_to)
{
    /* Allocate new tables; size is up_to+1 to use 1‑based indices */
    size_t sz = (size_t)up_to + 1;
    bool *new_lower = (bool*)calloc(sz, sizeof(bool));
    bool *new_upper = (bool*)calloc(sz, sizeof(bool));
    bool *used      = (bool*)calloc(sz, sizeof(bool));

    idx_t k = 1;      /* sequence index */
    idx_t cur = 1;    /* current candidate for a_k */

    while (cur <= up_to) {
        /* advance to the next unused integer */
        while (cur <= up_to && used[cur]) ++cur;
        if (cur > up_to) break;

        /* mark lower term a_k */
        new_lower[cur] = true;
        used[cur] = true;

        /* compute upper term b_k = a_k + k */
        idx_t upper = cur + k;
        if (upper <= up_to) {
            new_upper[upper] = true;
            used[upper] = true;
        }
        ++k;
    }

    /* Replace old tables (freeing any previous allocation) */
    free(lower_mem);
    free(upper_mem);
    free(used);
    lower_mem = new_lower;
    upper_mem = new_upper;
    meth_size = up_to;
}

/* OEIS sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* outside the offset */

    /* Ensure tables cover all indices up to n */
    if (meth_size < n) build_tables(n);

    seq_t count = 0;

    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d != 0) continue;         /* not a divisor */
        idx_t e = n / d;                  /* complementary factor */

        /* (d, e) as (lower, upper) */
        if (lower_mem[d] && upper_mem[e]) ++count;

        /* (e, d) as (lower, upper) if distinct */
        if (d != e && lower_mem[e] && upper_mem[d]) ++count;
    }

    return count;
}
