/*
 * A295358  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n-1) + b(n-2) - b(n-3), where a(0) = 1, a(1) = 3, a(2) = 5, b(0)
 *          = 2, b(1) = 4, b(2) = 6, and (a(n)) and (b(n)) are increasing
 *          complementary sequences.
 * https://oeis.org/A295358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* "
 * Sequence A295358
 * Complementary sequences a() and b() satisfy
 *   a(n) = a(n-1) + a(n-2) + b(n-1) + b(n-2) - b(n-3)
 *   a(0)=1, a(1)=3, a(2)=5
 *   b(0)=2, b(1)=4, b(2)=6
 *   a and b are increasing and constitute the partition of
 *   positive integers (the nth a(n) is the nth smallest
 *   unused integer after b(n-1) is chosen).
 */

/* Standard headers for size_t, malloc, realloc */
#include <stddef.h>
#include <stdlib.h>

/* Static storage for the sequences and auxiliary state */
static seq_t *a_cache   = NULL;   /* array of a values */
static seq_t *b_cache   = NULL;   /* array of b values */
static size_t cache_len = 0;      /* number of entries computed, indices [0 .. cache_len-1] */
static size_t alloc_cap = 0;      /* current allocated capacity */
static seq_t next_candidate = 7;  /* next integer to test for b */


/* Binary search for a membership test in the sorted array a_cache[0..len-1]  */
static int is_in_a(seq_t key, size_t len)
{
    size_t low  = 0;
    size_t high = len;
    while (low < high) {
        size_t mid = low + (high - low) / 2;
        if (a_cache[mid] < key)
            low = mid + 1;
        else
            high = mid;
    }
    return (low < len) && (a_cache[low] == key);
}


/* Ensure that the cache contains a(0)…a(n) and b(0)…b(n) */
static void ensure_cache(idx_t n)
{
    /* Initialisation on first use */
    if (cache_len == 0) {
        /* Allocate at least the initial three entries */
        alloc_cap = 3;
        a_cache = (seq_t *)malloc(sizeof(seq_t) * alloc_cap);
        b_cache = (seq_t *)malloc(sizeof(seq_t) * alloc_cap);
        if (!a_cache || !b_cache) return;   /* silently abort on OOM */

        a_cache[0] = (seq_t)1;  a_cache[1] = (seq_t)3;  a_cache[2] = (seq_t)5;
        b_cache[0] = (seq_t)2;  b_cache[1] = (seq_t)4;  b_cache[2] = (seq_t)6;
        cache_len = 3;
        next_candidate = (seq_t)7;
    }

    if ((idx_t)cache_len > n) return;   /* already computed */

    /* Grow the backing arrays if necessary */
    while (alloc_cap < (size_t)(n + 1)) {
        size_t new_cap = alloc_cap ? alloc_cap * 2 : 8;
        seq_t *new_a = (seq_t *)realloc(a_cache, sizeof(seq_t) * new_cap);
        seq_t *new_b = (seq_t *)realloc(b_cache, sizeof(seq_t) * new_cap);
        if (!new_a || !new_b) return;  /* silently abort on OOM */
        a_cache = new_a;
        b_cache = new_b;
        alloc_cap = new_cap;
    }

    /* Compute missing values */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        if (i < 3) {
            /* Already initialised */
            continue;
        }

        /* a(i) = a(i-1) + a(i-2) + b(i-1) + b(i-2) - b(i-3) */
        seq_t new_a = a_cache[i - 1] + a_cache[i - 2]
                    + b_cache[i - 1] + b_cache[i - 2]
                    - b_cache[i - 3];
        a_cache[i] = new_a;

        /* Find b(i): smallest positive integer not yet used in a_cache[0..i] */
        seq_t cand = next_candidate;
        while (is_in_a(cand, i + 1)) {
            cand = cand + (seq_t)1;
        }
        b_cache[i] = cand;
        next_candidate = cand + (seq_t)1;
    }

    cache_len = (size_t)(n + 1);
}


/* Public function following the required signature */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return a_cache[n];
}
