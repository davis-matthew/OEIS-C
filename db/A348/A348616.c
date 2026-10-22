/*
 * A348616  Number of ordered factorizations of n with adjacent equal factors.
 * https://oeis.org/A348616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A074206(n) – A348611(n) A074206(n) = number of ordered
 *            factorizations of n A348611(n) = number of such factorizations
 *            with no adjacent equal factors Computed by memoised recursion
 *            using divisor lists
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
/* cast operators for C++   */
#define PTR_CAST(type, p) static_cast<type>(p)
#define INT_CAST(type, v) static_cast<type>(v)
#else
#define PTR_CAST(type, p) ((type)(p))
#define INT_CAST(type, v) ((type)(v))
#endif

/* divisor data structures */
static int *div_sizes = NULL;          /* number of divisors >= 2 for each n */
static int **div_lists = NULL;         /* arrays of those divisors */
static idx_t div_max = 0;              /* highest n built */

/* ensure the divisor lists are constructed for all numbers <= N */
static void ensure_divisors(idx_t N)
{
    if (N <= div_max) return;

    /* free existing data */
    if (div_lists) {
        for (idx_t i = 0; i <= div_max; ++i)
            free(div_lists[i]);
        free(div_lists);
    }
    free(div_sizes);

    /* allocate new arrays up to N */
    div_max = N;
    div_sizes = PTR_CAST(int *, calloc((size_t)(div_max + 1), sizeof(int)));
    div_lists = PTR_CAST(int **, malloc((size_t)(div_max + 1) * sizeof(int *)));
    for (idx_t i = 0; i <= div_max; ++i)
        div_lists[i] = NULL;

    /* first pass: count divisors (>=2) for each number */
    for (idx_t d = 2; d <= div_max; ++d) {
        for (idx_t m = d; m <= div_max; m += d)
            ++div_sizes[m];
    }

    /* allocate per–number divisor arrays */
    for (idx_t i = 0; i <= div_max; ++i) {
        if (div_sizes[i] > 0)
            div_lists[i] = PTR_CAST(int *, malloc((size_t)div_sizes[i] * sizeof(int)));
    }

    /* second pass: fill the arrays */
    int *pos = PTR_CAST(int *, calloc((size_t)(div_max + 1), sizeof(int)));
    for (idx_t d = 2; d <= div_max; ++d) {
        for (idx_t m = d; m <= div_max; m += d) {
            int index = pos[m]++;
            div_lists[m][index] = (int)d;
        }
    }
    free(pos);
}

/* total ordered factorizations (A074206) */
static seq_t *Tcache = NULL;    /* memoisation array */
static idx_t Tmax = 0;          /* highest index cached */

/* recursive helper for total factorizations */
static seq_t total_factorizations(idx_t n)
{
    if (n == 1) return INT_CAST(seq_t, 1);   /* empty factorization counted once */

    ensure_divisors(n);

    if (n > Tmax || Tcache == NULL) {
        idx_t newmax = n;
        seq_t *newc = PTR_CAST(seq_t *, realloc(Tcache,
                            (size_t)(newmax + 1) * sizeof(seq_t)));
        if (!newc) return INT_CAST(seq_t, 0);   /* allocation failed */

        if (Tcache == NULL) {
            memset(newc, 0, (size_t)(newmax + 1) * sizeof(seq_t));
        } else {
            /* initialise new entries */
            for (idx_t i = Tmax + 1; i <= newmax; ++i)
                newc[i] = INT_CAST(seq_t, 0);
        }
        Tcache = newc;
        Tmax = newmax;
    }

    if (Tcache[n] != INT_CAST(seq_t, 0))
        return Tcache[n];

    seq_t res = INT_CAST(seq_t, 0);
    for (idx_t i = 0; i < INT_CAST(idx_t, div_sizes[n]); ++i) {
        int d = div_lists[n][i];
        res += total_factorizations(n / d);
    }
    Tcache[n] = res;
    return res;
}

/* factorizations with no adjacent equal factors (A348611) */
#define HASH_CAP (1 << 20)    /* power‑of‑two size; large enough for n<=10000 */

typedef struct {
    uint64_t key;  /* (n<<32) | last_factor */
    seq_t   value;
} cache_entry_t;

static cache_entry_t fact_cache[HASH_CAP] = {0,0};

/* recursive helper with memoisation */
static seq_t countWays(idx_t n, int last)
{
    if (n == 1) return INT_CAST(seq_t, 1);

    uint64_t key = ((uint64_t)n << 32) | (uint32_t)last;
    size_t h = key & (HASH_CAP - 1);

    /* linear probing lookup */
    while (fact_cache[h].key) {
        if (fact_cache[h].key == key)
            return fact_cache[h].value;
        h = (h + 1) & (HASH_CAP - 1);
    }

    seq_t res = INT_CAST(seq_t, 0);
    for (idx_t i = 0; i < INT_CAST(idx_t, div_sizes[n]); ++i) {
        int d = div_lists[n][i];
        if (d == last) continue;
        res += countWays(n / d, d);
    }

    fact_cache[h].key   = key;
    fact_cache[h].value = res;
    return res;
}

/* public function implementing a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return INT_CAST(seq_t, 0);    /* undefined index handling */

    ensure_divisors(n);

    seq_t total   = total_factorizations(n);
    seq_t without = countWays(n, 0);

    return total - without;
}
