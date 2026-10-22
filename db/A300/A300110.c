/*
 * A300110  Number of nX3 0..1 arrays with every element equal to 1, 2, 4, 5,
 *          6, 7 or 8 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A300110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) -7*a(n-2) +4*a(n-3) -4*a(n-4) +7*a(n-5)
 *            -26*a(n-6) * +26*a(n-7) +17*a(n-8) -5*a(n-9) -a(n-10)
 *            -40*a(n-11) +22*a(n-12) * -11*a(n-13) +14*a(n-14) +2*a(n-15)
 *            -2*a(n-16), for n > 17. * Initial values: * a(1)=1, a(2)=7,
 *            a(3)=15, a(4)=29, a(5)=63, a(6)=199, a(7)=629, * a(8)=1837,
 *            a(9)=5...
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Memoisation cache ---------------------------------------------------- */
static seq_t *cache = NULL;   /* array of computed terms, 1‑based */
static size_t cache_cap = 0;  /* capacity of cache array  */
static size_t cache_max = 0;  /* largest n that has been computed */

/* initialise base values up to n = 17 ---------------------------------- */
static void
init_base(void)
{
    const seq_t base[18] = {
        0ULL,                       /* index 0 unused */
        1ULL, 7ULL, 15ULL, 29ULL, 63ULL, 199ULL,
        629ULL, 1837ULL, 5397ULL, 15855ULL,
        46857ULL, 139131ULL, 411859ULL,
        1218209ULL, 3604353ULL, 10667225ULL,
        31574175ULL                 /* a(17) */
    };
    size_t need = 18;
    if (cache_cap < need) {
        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = need;
    }
    for (size_t i = 1; i < need; ++i)
        cache[i] = base[i];
    cache_max = 17;
}

/* compute term a(n) ---------------------------------------------------- */
seq_t a(idx_t n)
{
    /* offset 1 -------------------------------------------------------- */
    if (n == 0) return 0;  /* behaviour below offset does not matter */

    /* initialise cache if it has never been used --------------------- */
    if (!cache) init_base();

    /* ensure capacity up to n ----------------------------------------- */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap < cache_cap * 2) new_cap = cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* for small n return the pre‑computed values ---------------------- */
    if (n <= 17) return cache[(size_t)n];

    /* compute subsequent terms using the recurrence ------------------- */
    for (size_t i = cache_max + 1; i <= (size_t)n; ++i) {
        seq_t S = 0;
        S += cache[i-1] * 5;
        S += cache[i-3] * 4;
        S += cache[i-5] * 7;
        S += cache[i-7] * 26;
        S += cache[i-8] * 17;
        S += cache[i-12] * 22;
        S += cache[i-14] * 14;
        S += cache[i-15] * 2;

        seq_t T = 0;
        T += cache[i-2] * 7;
        T += cache[i-4] * 4;
        T += cache[i-6] * 26;
        T += cache[i-9] * 5;
        T += cache[i-10] * 1;
        T += cache[i-11] * 40;
        T += cache[i-13] * 11;
        T += cache[i-16] * 2;

        cache[i] = S - T;
    }
    cache_max = (size_t)n;
    return cache[(size_t)n];
}
