/*
 * A339932  a(n+1) = a(n-4-a(n)^2) + 1, starting with a(1) = a(2) = a(3) =
 *          a(4) = a(5) = 0.
 * https://oeis.org/A339932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=a(2)=a(3)=a(4)=a(5)=0
   a(n)   = a(n-4-a(n-1)^2) + 1   (n ≥ 6)  */
/* The function memoises values in a static array so it works for any
   call order.  It uses only the integer operations allowed in the
   problem statement and compiles as C11 and C++17 (with no `new`,
   `class` or `template`).  */

#include <stdlib.h>  /* malloc, realloc, abort */

static seq_t *a_cache          = NULL;   /* 1‑indexed array of computed terms   */
static idx_t  a_cache_size     = 0;      /* largest n that has been computed    */
static idx_t  a_cache_capacity = 0;      /* allocated capacity, >= a_cache_size */

/* Ensure that the cache can hold indices up to n. */
static void a_ensure_cache(idx_t n)
{
    if (n <= a_cache_capacity)
        return;

    idx_t new_cap = a_cache_capacity ? a_cache_capacity * 2 : 64;
    while (new_cap < n)
        new_cap <<= 1;          /* double until sufficient */

    /* realloc returns void*; cast explicitly for C++ compatibility. */
    seq_t *tmp = (seq_t *)realloc(a_cache, (new_cap + 1) * sizeof(seq_t));
    if (!tmp)
        abort();                /* memory allocation failed */

    a_cache          = tmp;
    a_cache_capacity = new_cap;
}

/* Return a(n) for the A339932 sequence. */
seq_t a(idx_t n)
{
    if (n < 1)              /* undefined for n < 1, return 0 */
        return 0;

    /* Extend the cache if necessary. */
    if (n > a_cache_size) {
        a_ensure_cache(n);

        for (idx_t i = a_cache_size + 1; i <= n; ++i) {
            if (i <= 5) {
                a_cache[i] = 0;
            } else {
                idx_t  prev_idx = i - 1;
                seq_t  prev_val = a_cache[prev_idx];

                /* Convert prev_val to idx_t for squaring; the value
                   is always small enough for current constraints. */
                idx_t offset = ((idx_t)prev_val) * ((idx_t)prev_val);
                idx_t k = prev_idx - 4 - offset;
                if (k < 1)
                    k = 1;        /* safety guard; recursion guarantees k >= 1 */

                a_cache[i] = a_cache[k] + 1;
            }
        }
        a_cache_size = n;
    }

    return a_cache[n];
}
