/*
 * A397557  A sequence constructed by greedily sampling the zeta distribution
 *          for parameter value 3 to minimize discrepancy.
 * https://oeis.org/A397557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Algorithm: Greedy sampling from the zeta(3) distribution.
 * At step n we choose the integer k maximizing
 *     n/(ζ(3)·k³) – c(k),
 * where c(k) is the number of times k has already appeared.
 * The sequence is produced incrementally and memoised so that any a(n)
 * may be requested in arbitrary order.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* ζ(3) ≈ 1.202056903159594, stored as NUM/DEN with high precision. */
static const unsigned long long ZETA_NUM = 1202056903159594ULL;
static const unsigned long long ZETA_DEN = 1000000000000000ULL;

/* Static storage for the running counts of each integer and the sequence itself. */
static idx_t *counts     = NULL;   /* counts[k] = occurrences of k so far (k >= 1) */
static size_t counts_cap = 0;
static idx_t max_k       = 0;      /* largest k that has been produced so far */

static seq_t *seq_arr   = NULL;    /* seq_arr[1..seq_len] holds the sequence */
static size_t seq_cap   = 0;
static idx_t seq_len    = 0;       /* number of terms stored */

/* Ensure the counts array has at least `need` slots (indexable up to need-1). */
static void ensure_counts_capacity(size_t need)
{
    if (need <= counts_cap) return;
    size_t new_cap = counts_cap ? counts_cap << 1 : 64;
    if (new_cap < need) new_cap = need;
    idx_t *new_arr = (idx_t *)malloc(new_cap * sizeof(idx_t));
    if (!new_arr) abort();
    if (counts) {
        memcpy(new_arr, counts, counts_cap * sizeof(idx_t));
        memset(new_arr + counts_cap, 0, (new_cap - counts_cap) * sizeof(idx_t));
        free(counts);
    } else {
        memset(new_arr, 0, new_cap * sizeof(idx_t));
    }
    counts = new_arr;
    counts_cap = new_cap;
}

/* Ensure the sequence array can hold at least `need` positions (indexable up to need-1). */
static void ensure_seq_capacity(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 64;
    if (new_cap < need) new_cap = need;
    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_arr) abort();
    if (seq_arr) {
        memcpy(new_arr, seq_arr, seq_len * sizeof(seq_t));
        free(seq_arr);
    }
    seq_arr = new_arr;
    seq_cap = new_cap;
}

/* Return the n-th term of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined, but safe for completeness */
    if (n <= seq_len) return seq_arr[n]; /* already computed */

    /* Generate terms up to n. */
    for (idx_t cur = seq_len + 1; cur <= n; ++cur) {
        /* Ensure buffers are large enough. */
        ensure_counts_capacity(max_k + 2);      /* need indices up to max_k+1 */
        ensure_seq_capacity(cur + 1);          /* need index cur */

        /* Find the best k for this position. */
        idx_t best_k  = 1;
        unsigned long long cube_best = 1ULL;
        __int128 best_diff = (__int128)cur * ZETA_DEN
                             - (__int128)counts[1] * ZETA_NUM * cube_best;

        for (idx_t k = 2; k <= max_k + 1; ++k) {
            unsigned long long cube_k = (unsigned long long)k * k * k;
            __int128 cur_diff = (__int128)cur * ZETA_DEN
                                - (__int128)counts[k] * ZETA_NUM * cube_k;

            /* Compare: cur_diff / (ZETA_NUM * cube_k)  vs  best_diff / (ZETA_NUM * cube_best) */
            __int128 left  = cur_diff * (__int128)cube_best;
            __int128 right = best_diff * (__int128)cube_k;
            if (left > right) {
                best_k    = k;
                cube_best = cube_k;
                best_diff = cur_diff;
            }
        }

        /* Record the chosen value. */
        ++counts[best_k];
        if (best_k > max_k) max_k = best_k;
        seq_arr[cur] = (seq_t)best_k;
    }

    seq_len = n;
    return seq_arr[n];
}
