/*
 * A299847  Number of nX3 0..1 arrays with every element equal to 0, 1, 2, 3,
 *          5, 7 or 8 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A299847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence for A299847 (n≥19) a(1)=4, a(2)=26,
 *            a(3)=93, a(4)=357, a(5)=1401, a(6)=5533, a(7)=21764, a(8)=85620,
 *            a(9)=336966, a(10)=1326498, a(11)=5221450, a(12)=20553311,
 *            a(13)=80905473, a(14)=318477196, a(15)=1253655191,
 *            a(16)=4934896919, a(17)=19425767155, a(18)=76467769656; for
 *            n≥19:...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Internal cache for memoised values */
static seq_t *cache = NULL;     /* alloc'd as 1‑based index, cache[0] unused */
static idx_t cache_cap = 0;     /* allocated size ( >= 1 ) */
static idx_t max_calculated = 0;/* largest n currently stored */

/* Initial values (as 64‑bit unsigned) for n = 1 … 18 */
static const uint64_t init_ull[18] = {
    4ULL,    26ULL,    93ULL,    357ULL,
    1401ULL, 5533ULL,  21764ULL, 85620ULL,
    336966ULL, 1326498ULL, 5221450ULL, 20553311ULL,
    80905473ULL, 318477196ULL, 1253655191ULL, 4934896919ULL,
    19425767155ULL, 76467769656ULL
};

/* Coefficients for the recurrence (n≥19) */
static const int coeff[16] = {
    6, -9, 3, 7, -31, 52, -51, 53,
    -100, 53, 65, 7, -48, -26, 18, 12
};

/* Ensure the cache has space up to and including index n */
static void ensure_cache(idx_t n)
{
    if (cache_cap >= n + 1)
        return;               /* enough space already */

    idx_t new_cap = cache_cap == 0 ? 16 : cache_cap;
    while (new_cap < n + 1)
        new_cap <<= 1;         /* grow by factor 2 */

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(1);               /* fatal: out of memory */

    /* zero new part for all integer types */
    if (new_cap > cache_cap)
        memset(tmp + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));

    cache = tmp;
    cache_cap = new_cap;
}

/**
 * Return the n‑th term of A299847.
 *
 * The sequence is memoised; subsequent calls are O(1).
 */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;               /* undefined for n=0 – return 0 for safety */

    ensure_cache(n);

    if (n <= max_calculated)
        return cache[n];        /* already computed */

    /* Compute missing terms up to n, increasing order. */
    for (idx_t i = max_calculated + 1; i <= n; ++i) {
        if (i <= 18) {
            /* initialise base values if not already set */
            if (cache[i] == 0)
                cache[i] = (seq_t)init_ull[i - 1];
        } else {
            /* use the empirical recurrence for i ≥ 19 */
            seq_t pos = 0;
            seq_t neg = 0;
            for (int k = 0; k < 16; ++k) {
                idx_t j = i - (k + 1);            /* predecessor index */
                seq_t term = cache[j];
                int c = coeff[k];
                if (c > 0)
                    pos += (seq_t)c * term;
                else
                    neg += (seq_t)(-c) * term;
            }
            cache[i] = pos - neg;   /* pos ≥ neg by definition of recurrence */
        }
    }
    max_calculated = n;
    return cache[n];
}
