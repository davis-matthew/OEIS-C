/*
 * A305118  a(n) = Sum_{k=0..n-1} ( 1 + a(k) * a(n-k-1) ) for n >= 1, a(0) =
 *          1.
 * https://oeis.org/A305118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = \sum_{k=0}^{n-1} (1 + a(k) * a(n-k-1)), a(0) = 1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 16, u64 n <= 30, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute a(n).  The result is stored in the static cache so the
   function is fast even for out‑of‑order calls. */
seq_t a(idx_t n) {
    static seq_t *cache = NULL;      /* cached values a(0..i)       */
    static size_t cap = 0;           /* current capacity of cache   */
    static idx_t last = 0;           /* highest index already computed */

    /* Allocate and initialise cache[0] if required. */
    if (n == 0) {
        if (cache == NULL) {
            cap = 8; /* arbitrary small initial capacity */
            cache = (seq_t *)malloc(cap * sizeof(seq_t));
            if (!cache)
                abort();            /* allocation failed */
            cache[0] = 1;          /* a(0) = 1 */
            last = 0;
        }
        return cache[0];
    }

    /* The cache may need to grow to accommodate index n. */
    if (n + 1 > cap) {
        size_t newcap = cap * 2;
        if (newcap < 8)
            newcap = 8;
        if (newcap < n + 1)
            newcap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp)
            abort();              /* memory allocation failed */
        cache = tmp;
        cap = newcap;
    }

    /* Compute all a(i) up to n that are not yet computed. */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 0; k < i; ++k) {
            sum += 1 + cache[k] * cache[i - 1 - k];
        }
        cache[i] = sum;
    }
    last = n > last ? n : last;

    return cache[n];
}
