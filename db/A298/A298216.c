/*
 * A298216  Number of nX3 0..1 arrays with every element equal to 0, 1, 2, 4
 *          or 5 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n‑1) + a(n‑3) – 13*a(n‑4) + 3*a(n‑5) – a(n‑6) +
 *            18*a(n‑7) – 4*a(n‑8) + 7*a(n‑9) – 7*a(n‑10) + 5*a(n‑11) –
 *            6*a(n‑12) – a(n‑13) – 4*a(n‑14) for n > 15 with initial terms
 *            a(1) = 4, a(2) = 13, a(3) = 20, a(4) = 40, a(5) = 89, a(6) =
 *            195, a(7) = 451, a(8) = 1046, a(9) = 2453, a(10) = 5810,...
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 50, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* a(idx_t n) returns the n-th term of A298216 (n≥1). */
seq_t
a(idx_t n)
{
    if (n == 0)                           /* out of range request */
        return (seq_t)0;

    /* Initial 15 terms – must remain in this order. */
    static const seq_t base[15] = {
        (seq_t)4,   (seq_t)13,  (seq_t)20,  (seq_t)40,
        (seq_t)89,  (seq_t)195, (seq_t)451, (seq_t)1046,
        (seq_t)2453,(seq_t)5810,(seq_t)13796,(seq_t)32903,
        (seq_t)78633,(seq_t)188163,(seq_t)450830
    };

    /* Static cache for already-computed values. */
    static seq_t *cache = NULL;   /* array 0‑based, a(n) at cache[n‑1] */
    static idx_t cache_size = 0;  /* number of stored terms */

    /* Determine how many terms must be available. */
    idx_t needed = n;
    if (needed < 15)                 /* we still need at least 15 terms for recurrence */
        needed = 15;

    /* Allocate or enlarge the cache if required. */
    if (cache == NULL || cache_size < needed) {
        seq_t *tmp = (seq_t *)realloc(cache,
                                      (size_t)needed * sizeof(seq_t));
        if (tmp == NULL)            /* allocation failed – abort gracefully */
            return (seq_t)0;
        cache = tmp;

        /* Fill any new positions. */
        for (idx_t i = cache_size; i < needed; ++i) {
            if (i < 15) {            /* copy the known initial terms */
                cache[i] = base[i];
            } else {                 /* recurrence for i≥15, i= n‑1 */
                idx_t idx = i + 1;   /* current n (1‑based) */
                cache[i] =
                    (seq_t)3  * cache[idx - 2]  /* a(n‑1) */
                  + (seq_t)1  * cache[idx - 4]  /* a(n‑3) */
                  - (seq_t)13 * cache[idx - 5]  /* a(n‑4) */
                  + (seq_t)3  * cache[idx - 6]  /* a(n‑5) */
                  - (seq_t)1  * cache[idx - 7]  /* a(n‑6) */
                  + (seq_t)18 * cache[idx - 8]  /* a(n‑7) */
                  - (seq_t)4  * cache[idx - 9]  /* a(n‑8) */
                  + (seq_t)7  * cache[idx - 10] /* a(n‑9) */
                  - (seq_t)7  * cache[idx - 11] /* a(n‑10) */
                  + (seq_t)5  * cache[idx - 12] /* a(n‑11) */
                  - (seq_t)6  * cache[idx - 13] /* a(n‑12) */
                  - (seq_t)1  * cache[idx - 14] /* a(n‑13) */
                  - (seq_t)4  * cache[idx - 15];/* a(n‑14) */
            }
        }
        cache_size = needed;
    }

    return cache[n - 1];
}
