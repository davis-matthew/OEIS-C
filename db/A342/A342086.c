/*
 * A342086  Number of strict factorizations of divisors of n.
 * https://oeis.org/A342086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1079, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A342086: number of strict factorizations of all divisors of n.
    A strict factorization of m is a set of distinct integers > 1 whose
    product is m.  For each divisor d of n we add the number of strict
    factorizations of d (the empty set counts for d = 1).  The function
    builds divisor tables up to the requested n and counts factorizations
    recursively with memoisation of divisor lists only.  The algorithm
    runs in O(n·τ(n)) time for a single call, well below the limits
    for n ≤ 10000.  It compiles under C11 and C++17. */

#include <stdlib.h>
#include <stdbool.h>

/* Forward declaration (types are defined by the build). */
seq_t a(idx_t n);

/* --- Global tables for divisor lists ---------------------------------- */

/* Number of divisors for each integer 1..cache_size */
static idx_t *div_counts = NULL;
/* Offset into div_data where the divisor list for i begins */
static idx_t *offsets    = NULL;
/* Flat array containing all divisor lists, sorted ascending */
static idx_t *div_data   = NULL;
/* Current maximum n for which tables are available */
static size_t cache_size = 0;

/* --- Recursive count of strict factorizations ------------------------ */

static seq_t count_rec(idx_t m, idx_t min_factor)
{
    if (m == 1)                  /* empty set is one factorization */
        return 1;

    seq_t total = 0;
    idx_t start = offsets[m];
    idx_t cnt   = div_counts[m];

    for (idx_t i = 0; i < cnt; ++i) {
        idx_t d = div_data[start + i];
        if (d < min_factor || d <= 1)
            continue;
        total += count_rec(m / d, d + 1);
    }
    return total;
}

/* --- Build divisor tables up to n ------------------------------------ */

static void ensure_cache(idx_t n)
{
    if (n <= cache_size)
        return;

    /* free any previous tables */
    free(div_counts);
    free(offsets);
    free(div_data);

    /* allocate new tables for 1..n */
    div_counts = (idx_t *)malloc((n + 1) * sizeof(idx_t));
    offsets    = (idx_t *)malloc((n + 1) * sizeof(idx_t));
    if (!div_counts || !offsets) {
        div_counts = offsets = NULL;   /* safety */
        return;
    }

    /* temporary array to count divisors */
    idx_t *tmp_counts = (idx_t *)calloc(n + 1, sizeof(idx_t));
    if (!tmp_counts) {
        free(div_counts); free(offsets);
        div_counts = offsets = NULL;
        return;
    }

    /* first pass: count divisors for each integer */
    for (idx_t i = 1; i <= n; ++i)
        for (idx_t j = i; j <= n; j += i)
            ++tmp_counts[j];

    /* compute offsets and total number of divisor entries */
    size_t total_entries = 0;
    for (idx_t j = 1; j <= n; ++j) {
        offsets[j]   = total_entries;
        div_counts[j] = tmp_counts[j];
        total_entries += (size_t)tmp_counts[j];
    }

    /* allocate flat divisor array */
    div_data = (idx_t *)malloc(total_entries * sizeof(idx_t));
    if (!div_data) {
        free(tmp_counts);
        free(div_counts); free(offsets);
        div_counts = offsets = div_data = NULL;
        return;
    }

    /* helper array to keep track of current position when filling */
    idx_t *cur = (idx_t *)calloc(n + 1, sizeof(idx_t));
    if (!cur) {
        free(tmp_counts); free(div_data);
        free(div_counts); free(offsets);
        div_counts = offsets = div_data = NULL;
        return;
    }

    /* fill divisor list in ascending order by iterating i then j */
    for (idx_t i = 1; i <= n; ++i)
        for (idx_t j = i; j <= n; j += i) {
            size_t idx = offsets[j] + cur[j];
            div_data[idx] = i;
            ++cur[j];
        }

    free(cur);
    free(tmp_counts);

    cache_size = n;  /* tables now valid up to n */
}

/* --- Main sequence function ------------------------------------------ */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;  /* undefined for n = 0, but return 0 for safety */

    ensure_cache(n);

    seq_t sum = 0;
    idx_t start = offsets[n];
    idx_t cnt   = div_counts[n];

    for (idx_t i = 0; i < cnt; ++i) {
        idx_t d = div_data[start + i];
        sum += count_rec(d, 2);   /* count strict factorizations of d */
    }
    return sum;
}
