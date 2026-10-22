/*
 * A395420  Numbers k = x + y such that there exists sigma(x) + sigma(y) =
 *          3*(x+y).
 * https://oeis.org/A395420
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Computation of A395420: numbers k = x + y such that sigma(x)+sigma(y)=3*k.
 * The algorithm precomputes divisor sums up to a sieve limit,
 * then finds all pairs (x,y) with sigma(x)-3x + sigma(y)-3y = 0,
 * collects their sums k=x+y, and stores the resulting k values
 * in increasing order.  a(idx_t n) returns the nth term (offset 1).
 */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* A pair containing the value f(n) = sigma(n)-3n and the index n */
typedef struct {
    int64_t f;
    idx_t idx;
} pair_t;

/* Static state for memoisation */
static idx_t   g_limit = 0;            /* current sieve limit            */
static seq_t * g_terms = NULL;         /* array of computed terms         */
static idx_t   g_terms_count = 0;      /* number of terms stored          */

/* Comparison function for qsort on pair_t by f */
static int cmp_pair(const void *a, const void *b)
{
    const pair_t *pa = (const pair_t *)a;
    const pair_t *pb = (const pair_t *)b;
    if (pa->f < pb->f) return -1;
    if (pa->f > pb->f) return 1;
    return 0;
}

/* Precompute the sequence until at least need_n terms are available. */
static void precompute(idx_t need_n)
{
    if (g_terms_count >= need_n) return;   /* already enough terms */

    if (g_limit == 0) g_limit = 5000000;   /* start with 5 M */

    while (1) {
        /* Allocate sigma array */
        uint64_t *sigma = (uint64_t *)calloc((size_t)g_limit + 1, sizeof(uint64_t));
        if (!sigma) exit(1);

        /* Sieve divisor sums */
        for (idx_t i = 1; i <= g_limit; ++i) {
            for (idx_t j = i; j <= g_limit; j += i) {
                sigma[j] += i;
            }
        }

        /* Build array of (f, index) */
        pair_t *arr = (pair_t *)calloc((size_t)g_limit, sizeof(pair_t));
        if (!arr) { free(sigma); exit(1); }

        for (idx_t i = 1; i <= g_limit; ++i) {
            arr[i - 1].f   = (int64_t)sigma[i] - (int64_t)3 * (int64_t)i;
            arr[i - 1].idx = i;
        }
        free(sigma);

        /* Sort by f */
        qsort(arr, (size_t)g_limit, sizeof(pair_t), cmp_pair);

        /* Mark good sums k = x + y */
        size_t maxSum = (size_t)g_limit * 2;
        bool *good = (bool *)calloc(maxSum + 1, sizeof(bool));
        if (!good) { free(arr); exit(1); }

        size_t left = 0, right = (size_t)g_limit - 1;
        while (left < right) {
            int64_t s = arr[left].f + arr[right].f;
            if (s < 0) {
                ++left;
            } else if (s > 0) {
                --right;
            } else { /* s == 0 */
                /* Find contiguous block of equal f at left */
                size_t l_start = left;
                size_t l_end   = left;
                while (l_end + 1 < right && arr[l_end + 1].f == arr[l_start].f) ++l_end;

                /* Find contiguous block of equal -f at right */
                size_t r_start = right;
                size_t r_end   = right;
                while (r_start > l_start && arr[r_start - 1].f == arr[r_start].f) --r_start;

                /* Cross product of the two blocks */
                for (size_t i = l_start; i <= l_end; ++i) {
                    for (size_t j = r_start; j <= r_end; ++j) {
                        idx_t k = arr[i].idx + arr[j].idx;
                        if ((size_t)k <= maxSum) good[(size_t)k] = true;
                    }
                }

                left  = l_end + 1;
                right = r_start - 1;
            }
        }

        /* Gather sequence terms in increasing order of k */
        size_t cap = 20000;           /* initial capacity for terms array */
        g_terms = (seq_t *)realloc(g_terms, cap * sizeof(seq_t));
        if (!g_terms) exit(1);

        idx_t count = 0;
        for (size_t k = 2; k <= maxSum; ++k) {
            if (good[k]) {
                if (count >= (idx_t)cap) {
                    cap *= 2;
                    g_terms = (seq_t *)realloc(g_terms, cap * sizeof(seq_t));
                    if (!g_terms) exit(1);
                }
                g_terms[count++] = (seq_t)k;
            }
        }
        g_terms_count = count;

        free(arr);
        free(good);

        if (g_terms_count >= need_n) break;   /* enough terms gathered */

        /* Not enough terms: increase limit and recompute */
        g_limit *= 2;
    }
}

/* Public function: returns a(n) for n >= 1 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* safety for out of bounds */
    precompute(n);
    return g_terms[n - 1];
}
