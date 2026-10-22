/*
 * A378698  Number of compositions of n into parts whose sizes are Fibonacci
 *          or Lucas numbers.
 * https://oeis.org/A378698
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of compositions of n into parts that are Fibonacci
 *            or Lucas numbers. Recurrence: a(0) = 1 a(n) = Σ_{s∈S, s≤n}
 *            a(n−s) (S = {Fibonacci numbers ≥1} ∪ {Lucas numbers ≥1}) where
 *            a(n) counts ordered sums of positive integers from S that sum to
 *            n. The implementation memoizes results in a s...
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;     /* memoized DP array */
static size_t cache_cap = 0;    /* current allocated capacity */
static idx_t cached_max = 0;    /* highest index currently computed */

static void ensure_cache(idx_t n)
{
    /* Expand cache storage if necessary */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 1;
        if (new_cap <= (size_t)n) new_cap = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Initialize base case a(0) = 1 if not done yet */
    if (cached_max == 0 && cache[0] == (seq_t)0) {
        cache[0] = (seq_t)1;
    }

    /* If n is already computed, nothing more to do */
    if (n <= cached_max) return;

    /* Generate all Fibonacci and Lucas numbers ≤ n */
    idx_t parts[256];    /* temporary storage for raw parts */
    size_t parts_cnt = 0;

    /* Fibonacci numbers (positive only, unique already handled later) */
    idx_t f_prev = 1, f_curr = 1;
    while (f_prev <= n) {
        parts[parts_cnt++] = f_prev;
        idx_t f_next;
        if (f_curr > n - f_prev) f_next = n + 1; else f_next = f_prev + f_curr;
        f_prev = f_curr;
        f_curr = f_next;
    }

    /* Lucas numbers (positive only) */
    idx_t l_prev = 2, l_curr = 1;
    while (l_curr <= n) {
        parts[parts_cnt++] = l_curr;
        idx_t l_next;
        if (l_prev > n - l_curr) l_next = n + 1; else l_next = l_prev + l_curr;
        l_prev = l_curr;
        l_curr = l_next;
    }

    /* Sort the part list ascending (simple insertion sort, small size) */
    for (size_t i = 1; i < parts_cnt; ++i) {
        idx_t key = parts[i];
        size_t j = i;
        while (j > 0 && parts[j - 1] > key) {
            parts[j] = parts[j - 1];
            --j;
        }
        parts[j] = key;
    }

    /* Remove duplicates and store in uniq_parts */
    idx_t uniq_parts[256];
    size_t uniq_cnt = 0;
    for (size_t i = 0; i < parts_cnt; ++i) {
        if (uniq_cnt == 0 || parts[i] != uniq_parts[uniq_cnt - 1]) {
            uniq_parts[uniq_cnt++] = parts[i];
        }
    }

    /* Fill DP values from cached_max+1 up to n */
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (size_t j = 0; j < uniq_cnt; ++j) {
            idx_t part = uniq_parts[j];
            if (part > i) break;
            sum += cache[i - part];
        }
        cache[i] = sum;
    }

    cached_max = n;
}

seq_t a(idx_t n)
{
    /* Ensure the DP table is valid up to n */
    ensure_cache(n);
    return cache[n];
}
