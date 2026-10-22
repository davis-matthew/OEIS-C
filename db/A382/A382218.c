/*
 * A382218  Fixed points of A382217.
 * https://oeis.org/A382218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A382218 (Fixed points of A382217)
   This implementation enumerates, for each factorial‑base width w,
   all digit vectors (d1,…,dw) that satisfy the condition:
          for i = 2..w there exists j < i such that d_j > i-j.
   Each valid vector is converted to its factorial‑base value and
   stored in a static cache.  The cache is grown until it contains at
   least n terms, so that any subsequent call to a(n) returns in
   constant time.  The enumeration proceeds in increasing order of w,
   guaranteeing that the global list remains sorted.
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static seq_t fact_arr[25];          /* factorial values up to 24! */
static int digits[25];              /* current digit vector */
static seq_t *cache = NULL;         /* cache of sequence values */
static size_t cache_size = 0;       /* number of values stored */
static size_t cache_cap = 0;        /* allocated capacity of cache */

/* Ensure the cache has room for at least 'needed' elements.  */
static void cache_reserve(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!newp) abort();               /* allocation failure */
    cache = newp;
    cache_cap = new_cap;
}

/* Append a value to the cache. */
static void cache_append(seq_t val)
{
    if (cache_size == cache_cap)
        cache_reserve(cache_size + 1);
    cache[cache_size++] = val;
}

/* Recursive enumeration of all valid digit vectors of given width. */
static void dfs_gen(int pos, int w, seq_t cur_val)
{
    if (pos > w) {                    /* finished one vector */
        cache_append(cur_val);
        return;
    }

    int min_d = (pos == 1) ? 1 : 0;    /* leading digit must be >0 */
    int max_d = w - pos + 1;           /* upper bound for digit[pos] */

    for (int d = min_d; d <= max_d; ++d) {
        digits[pos] = d;

        if (pos >= 2) {
            bool ok = false;
            for (int j = 1; j < pos; ++j) {
                if (digits[j] > pos - j) { ok = true; break; }
            }
            if (!ok) continue;         /* prune if condition fails */
        }

        seq_t next_val = cur_val + fact_arr[w - pos + 1] * (seq_t)d;
        dfs_gen(pos + 1, w, next_val);
    }
}

/* Compute the factorial values up to 24! once. */
static bool fact_inited = false;
static void init_factorials(void)
{
    if (fact_inited) return;
    fact_arr[0] = (seq_t)1;
    for (int i = 1; i < 25; ++i)
        fact_arr[i] = fact_arr[i - 1] * (seq_t)i;
    fact_inited = true;
}

/* Public API required by the build. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    init_factorials();

    static unsigned long current_width = 0;  /* width currently processed */

    /* Extend the cache until it contains at least n terms. */
    while (cache_size < n) {
        ++current_width;
        if (current_width >= 25) {         /* safety guard */
            break;                         /* cannot extend further */
        }
        dfs_gen(1, (int)current_width, (seq_t)0);
    }

    if (n <= cache_size)
        return cache[n - 1];
    return (seq_t)0;                       /* out of range for very large n */
}
