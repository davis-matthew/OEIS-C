/*
 * A340224  a(n+1) = a(n-a(n)*(a(n)-1)/2) + 1, starting with a(1) = 0.
 * https://oeis.org/A340224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=0; for n≥1,
   a(n+1) = a( n - C( a(n), 2 ) ) + 1   where C(a,2)=a*(a-1)/2
   (indices are 1‑based).  The implementation uses memoisation, storing
   previously computed terms in a growable static array. */
#include <stddef.h>
#include <stdlib.h>

/* static state holding the memoised terms */
static seq_t *cache = NULL;     /* a[0..computed-1] holds a(1)..a(computed)   */
static size_t cap = 0;           /* current allocation size                  */
static idx_t computed = 0;       /* number of terms currently stored          */

/* Grow cache to at least 'needed' elements. */
static void ensure_capacity(size_t needed)
{
    if (cap < needed) {
        size_t new_cap = (cap == 0 ? 1 : cap * 2);
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp) {
            cache = tmp;
            cap = new_cap;
        }
    }
}

/* Return the n-th term of A340224 (1‑based).  The function correctly
   handles calls in any order of n. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined by definition – guard only */

    /* Initialise cache with a(1)=0 on first call. */
    if (computed == 0) {
        ensure_capacity(1);
        cache[0] = 0;
        computed = 1;
    }

    /* Return cached value if already computed. */
    if (n <= computed)
        return cache[n - 1];

    /* Compute terms up to n, iteratively. */
    for (idx_t i = computed; i < n; ++i) {
        seq_t curr = cache[i - 1];                     /* a(i)   */
        seq_t bin  = curr * (curr - 1) / 2;             /* C(a(i),2) */
        idx_t back = i - (idx_t) bin;                  /* back‑ref index (1‑based) */

        seq_t next = cache[back - 1] + 1;               /* a(i+1)  */

        ensure_capacity(computed + 1);
        cache[computed++] = next;
    }

    return cache[n - 1];
}
