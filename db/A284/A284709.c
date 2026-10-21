/*
 * A284709  Number of maximal matchings in the wheel graph on n nodes.
 * https://oeis.org/A284709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-2) + 2*a(n-3) - 3*a(n-4) - 4*a(n-5) + 2*a(n-7) +
 *            a(n-8) Initial values: a(1)=2, a(2)=1, a(3)=4, a(4)=3, a(5)=10,
 *            a(6)=10, a(7)=20, a(8)=28
 * Range:     u8 n <= 13, u16 n <= 30, u32 n <= 67, u64 n <= 143,
 *            u128 n <= 298, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Fixed‑size cache for n up to 10 000.  It is 1‑indexed. */
static seq_t  static_cache[10001];
static idx_t static_cached = 0;          /* highest n cached in static_cache */

/* Dynamic cache for n > 10 000.  It is also 1‑indexed. */
static seq_t *dyn_cache   = NULL;
static idx_t dyn_cached   = 0;           /* highest n cached in dyn_cache */

/* Initialise the fixed cache with the first eight values. */
static void init_static(void)
{
    if (static_cached > 0) return;
    static_cache[1] = (seq_t)2;
    static_cache[2] = (seq_t)1;
    static_cache[3] = (seq_t)4;
    static_cache[4] = (seq_t)3;
    static_cache[5] = (seq_t)10;
    static_cache[6] = (seq_t)10;
    static_cache[7] = (seq_t)20;
    static_cache[8] = (seq_t)28;
    static_cached   = 8;
}

/* Initialise the dynamic cache with the first eight values. */
static void init_dynamic(void)
{
    if (dyn_cache != NULL) return;
    dyn_cache = (seq_t *)malloc(sizeof(seq_t) * 9);   /* indices 0..8 */
    dyn_cache[0] = 0;                                 /* unused */
    dyn_cache[1] = (seq_t)2;
    dyn_cache[2] = (seq_t)1;
    dyn_cache[3] = (seq_t)4;
    dyn_cache[4] = (seq_t)3;
    dyn_cache[5] = (seq_t)10;
    dyn_cache[6] = (seq_t)10;
    dyn_cache[7] = (seq_t)20;
    dyn_cache[8] = (seq_t)28;
    dyn_cached  = 8;
}

/* Compute a(n) using the recurrence.  The function guarantees that
   all terms up to n are present in the appropriate cache.  It works
   for any order of calls. */
static void ensure_cache(idx_t n)
{
    if (n == 0) return;               /* undefined, nothing to compute */

    if (n <= 10000) {                 /* use the static cache */
        init_static();
        if (n <= static_cached) return;
        for (idx_t i = static_cached + 1; i <= n; ++i) {
            seq_t t2 = static_cache[i - 2];
            seq_t t3 = static_cache[i - 3];
            seq_t t4 = static_cache[i - 4];
            seq_t t5 = static_cache[i - 5];
            seq_t t7 = static_cache[i - 7];
            seq_t t8 = static_cache[i - 8];
            seq_t val = (seq_t)3 * t2 + (seq_t)2 * t3
                      - (seq_t)3 * t4 - (seq_t)4 * t5
                      + (seq_t)2 * t7 + t8;
            static_cache[i] = val;
        }
        static_cached = n;
    } else {                          /* use the dynamic cache */
        init_dynamic();
        if (n <= dyn_cached) return;
        /* Expand the dynamic array if necessary */
        if (dyn_cache == NULL) {
            init_dynamic();
        }
        if (n >= dyn_cached) {
            seq_t *new_ptr =
                (seq_t *)realloc(dyn_cache, sizeof(seq_t) * (size_t)(n + 1));
            if (new_ptr == NULL)       /* allocation failed */
                return;
            dyn_cache = new_ptr;
            for (idx_t i = dyn_cached + 1; i <= n; ++i) {
                seq_t t2 = dyn_cache[i - 2];
                seq_t t3 = dyn_cache[i - 3];
                seq_t t4 = dyn_cache[i - 4];
                seq_t t5 = dyn_cache[i - 5];
                seq_t t7 = dyn_cache[i - 7];
                seq_t t8 = dyn_cache[i - 8];
                seq_t val = (seq_t)3 * t2 + (seq_t)2 * t3
                          - (seq_t)3 * t4 - (seq_t)4 * t5
                          + (seq_t)2 * t7 + t8;
                dyn_cache[i] = val;
            }
            dyn_cached = n;
        }
    }
}

/* Public function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, return 0 for safety */
    ensure_cache(n);
    if (n <= 10000) return static_cache[n];
    return dyn_cache[n];
}
