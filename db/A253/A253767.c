/*
 * A253767  Partial sums of A247666.
 * https://oeis.org/A253767
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Let b(n) = A247666(n). The recursion for b(n) is: b(0) = 1, b(1)
 *            = 7 b(2k) = b(k) For odd n = 2k+1: if k is even: b(n) = 7 * b(k)
 *            else (k is odd): b(n) = 3 * b(k) + 4 * b((k-1)/2) The desired
 *            sequence a(n) is the prefix sum of b: a(0) = 1 a(n) = a(n-1) +
 *            b(n) for n ≥ 1 The function below implemen...
 * Range:     u8 n <= 8, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Helper arrays for memoisation.  They grow as large as needed. */
static seq_t *b_cache   = NULL;   // cache for b(n)
static bool *b_used     = NULL;
static size_t b_cap     = 0;

static seq_t *a_cache   = NULL;   // cache for a(n)
static bool *a_used     = NULL;
static size_t a_cap     = 0;

static void ensure_b_cache(idx_t n)
{
    if (n < b_cap) return;
    size_t new_cap = (size_t)n + 1;
    b_cache = (seq_t*)realloc(b_cache, new_cap * sizeof(seq_t));
    b_used  = (bool *)realloc(b_used,  new_cap * sizeof(bool));
    if (b_used) {
        for (size_t i = b_cap; i < new_cap; ++i) b_used[i] = false;
    }
    b_cap = new_cap;
}

static void ensure_a_cache(idx_t n)
{
    if (n < a_cap) return;
    size_t new_cap = (size_t)n + 1;
    a_cache = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
    a_used  = (bool *)realloc(a_used,  new_cap * sizeof(bool));
    if (a_used) {
        for (size_t i = a_cap; i < new_cap; ++i) a_used[i] = false;
    }
    a_cap = new_cap;
}

static seq_t get_b(idx_t n)
{
    ensure_b_cache(n);
    if (b_used[n]) return b_cache[n];

    seq_t result;
    if (n == 0) {
        result = (seq_t)1;               /* b(0) = 1 */
    } else if (n == 1) {
        result = (seq_t)7;               /* b(1) = 7 */
    } else if ((n & 1) == 0) {          /* even */
        result = get_b(n >> 1);         /* b(2k) = b(k) */
    } else {                            /* odd n = 2k+1 */
        idx_t k = (n - 1) >> 1;         /* k = (n-1)/2 */
        if ((k & 1) == 0) {             /* k even */
            result = (seq_t)7 * get_b(k); /* b(n) = 7 * b(k) */
        } else {                        /* k odd */
            idx_t k2 = (k - 1) >> 1;     /* (k-1)/2 */
            result = (seq_t)3 * get_b(k) + (seq_t)4 * get_b(k2);
        }
    }

    b_cache[n] = result;
    b_used[n]  = true;
    return result;
}

static seq_t get_a(idx_t n)
{
    ensure_a_cache(n);
    if (a_used[n]) return a_cache[n];

    seq_t result;
    if (n == 0) {
        result = (seq_t)1;                           /* a(0) = 1 */
    } else {
        result = get_a(n - 1) + get_b(n);             /* a(n) = a(n-1) + b(n) */
    }

    a_cache[n] = result;
    a_used[n]  = true;
    return result;
}

seq_t a(idx_t n)
{
    return get_a(n);
}
