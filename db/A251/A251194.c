/*
 * A251194  Number of (n+1) X (1+1) 0..1 arrays with no 2 X 2 subblock having
 *          the minimum of its diagonal elements less than the absolute
 *          difference of its antidiagonal elements.
 * https://oeis.org/A251194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) - a(n-3) - a(n-4) (n ≥ 5) * Initial
 *            values: a(1)=10, a(2)=25, a(3)=64, a(4)=164
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 22, u64 n <= 45, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *seq_cache   = NULL;
static idx_t seq_cache_sz = 0;     /* number of stored terms */
static idx_t seq_cache_cap = 0;    /* allocated capacity */

/* Ensure the cache can hold 'needed' elements */
static void seq_ensure_cache(idx_t needed)
{
    if (needed <= seq_cache_cap)
        return;

    idx_t new_cap = seq_cache_cap ? seq_cache_cap : 4;
    while (new_cap < needed)
        new_cap <<= 1;

    seq_t *new_ptr;
#ifdef __cplusplus
    new_ptr = reinterpret_cast<seq_t*>(realloc(seq_cache, new_cap * sizeof(seq_t)));
#else
    new_ptr = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
#endif
    if (!new_ptr)
        abort();  /* allocation failed */

    seq_cache = new_ptr;
    seq_cache_cap = new_cap;
}

/* Initialize the cache with the first four terms. */
static void seq_init_cache(void)
{
    if (seq_cache_cap == 0) {
        seq_ensure_cache(4);
        seq_cache[0] = (seq_t)10;   /* a(1) */
        seq_cache[1] = (seq_t)25;   /* a(2) */
        seq_cache[2] = (seq_t)64;   /* a(3) */
        seq_cache[3] = (seq_t)164;  /* a(4) */
        seq_cache_sz = 4;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined; safe fallback */

    seq_init_cache();

    if (n > seq_cache_cap)
        seq_ensure_cache(n);

    if (n > seq_cache_sz) {
        for (idx_t i = seq_cache_sz; i < n; ++i) {
            /* recurrence: a(i+1) = 2*a(i) + 2*a(i-1) - a(i-2) - a(i-3) */
            seq_t term = seq_cache[i-1] + seq_cache[i-1]   /* 2*a(i)   */
                       + seq_cache[i-2] + seq_cache[i-2]   /* 2*a(i-1) */
                       - seq_cache[i-3]                    /* -a(i-2)  */
                       - seq_cache[i-4];                   /* -a(i-3)  */
            seq_cache[i] = term;
        }
        seq_cache_sz = n;
    }

    return seq_cache[n-1];
}
