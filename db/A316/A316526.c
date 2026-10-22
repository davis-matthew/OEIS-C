/*
 * A316526  a(n) = 122880*6^n - 307200*5^n + 264960*4^n - 90240*3^n + 9844*2^n
 *          - 122.
 * https://oeis.org/A316526
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 122880*6^n - 307200*5^n + 264960*4^n - 90240*3^n +
 *            9844*2^n - 122. Recurrence a(n) = 21*a(n-1) - 175*a(n-2) +
 *            735*a(n-3) - 1624*a(n-4) + 1764*a(n-5) - 720*a(n-6), for n > 5,
 *            with initial values a(0)=122, a(1)=9966, a(2)=210134,
 *            a(3)=2741670, a(4)=27930182, a(5)=245220486
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 6, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_cache = NULL;       /* dynamic cache of computed terms */
static size_t cache_capacity = 0;     /* length of allocated block */
static idx_t  max_computed  = 0;      /* highest index known in cache */

/* ensure there is room for indices up to n */
static void ensure_cache(idx_t n)
{
    if (cache_capacity > (size_t)n)
        return;
    size_t new_cap = cache_capacity ? cache_capacity : 16;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    cache_capacity = new_cap;
}

/* initialise cache with the first six terms of the sequence */
static void init_cache(void)
{
    if (cache_capacity == 0) {
        cache_capacity = 16;
        seq_cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        /* pre‑compute the first six terms */
        static const seq_t init_vals[6] = {
            (seq_t)122,
            (seq_t)9966,
            (seq_t)210134,
            (seq_t)2741670,
            (seq_t)27930182,
            (seq_t)245220486
        };
        for (idx_t i = 0; i < 6; ++i)
            seq_cache[i] = init_vals[i];
        max_computed = 5;          /* index of the last stored value */
    }
}

/* ------------------------------------------------------------ */
/* public function required by the specification               */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* initialise cache lazily */
    init_cache();

    if (n <= max_computed)
        return seq_cache[n];

    /* grow cache if necessary */
    ensure_cache(n);

    /* iterate from the next missing index up to n, filling cache */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t v =
              (seq_t)21   * seq_cache[i-1]
            - (seq_t)175  * seq_cache[i-2]
            + (seq_t)735  * seq_cache[i-3]
            - (seq_t)1624 * seq_cache[i-4]
            + (seq_t)1764 * seq_cache[i-5]
            - (seq_t)720  * seq_cache[i-6];
        seq_cache[i] = v;
    }

    max_computed = n;
    return seq_cache[n];
}
