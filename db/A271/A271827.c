/*
 * A271827  Expansion of (x^5-2*x^4+2*x^3-x+1)/(x^4-2*x^3+3*x^2-3*x+1).
 * https://oeis.org/A271827
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 2*Σ_{k=1..n} Σ_{j=0..(n-k)/2}
 *            binomial(k+1,j)*binomial(n-k-j-1,n-k-2*j). Recurrence: a(n) =
 *            3*a(n-1) - 3*a(n-2) + 2*a(n-3) - a(n-4) (for n ≥ 6). Initial
 *            values: a(0)=1, a(1)=2, a(2)=3, a(3)=7, a(4)=13, a(5)=23
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the first six terms explicitly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)7;
    if (n == 4) return (seq_t)13;
    if (n == 5) return (seq_t)23;

    /* Static cache for computed values. */
    static seq_t *cache = NULL;
    static idx_t cap = 0;      /* allocated capacity (number of elements) */
    static idx_t last = 0;     /* number of elements actually stored (up to last-1) */

    /* Grow cache if it cannot hold index n. */
    if (cap <= n) {
        idx_t new_cap = cap ? cap * 2 : 64;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;      /* Out‑of‑memory: return zero as a safe fallback. */
        cache = tmp;
        cap = new_cap;
    }

    /* Initialize cache with the first six terms if not yet done. */
    if (last == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)7;
        cache[4] = (seq_t)13;
        cache[5] = (seq_t)23;
        last = 6;
    }

    /* Compute all missing terms up to n using the recurrence. */
    for (idx_t i = last; i <= n; ++i) {
        cache[i] = (seq_t)3 * cache[i-1]
                 - (seq_t)3 * cache[i-2]
                 + (seq_t)2 * cache[i-3]
                 -       cache[i-4];
    }
    last = n + 1;          /* now cache[0 .. n] are valid */

    return cache[n];
}
