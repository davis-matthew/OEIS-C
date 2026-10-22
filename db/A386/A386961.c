/*
 * A386961  Expansion of e.g.f. (x^2/(1-x)^3)*exp(x/(1-x)).
 * https://oeis.org/A386961
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the recurrence * * (n-2)*a(n) = 2*(n-1)*n*a(n-1)
 *            - (n-1)^2*n*a(n-2) * * Initial values (offset 0): * a(0) = 0,
 *            a(1) = 0, a(2) = 2 * * The function caches previously computed
 *            values to allow arbitrary * access order without re-computation.
 *            Intermediate calculations are * performed i...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* External types supplied by the build environment */
typedef uint64_t idx_t;   /* NOTE: the build replaces these types as needed */

/* Public function prototype (definition follows) */
seq_t a(idx_t n);

/* Static cache for memoisation */
static seq_t *cache = NULL;        /* allocated array of a(i) values   */
static size_t cache_cap = 0;       /* current allocated capacity       */
static idx_t  computed_to = 0;     /* largest n that has been computed */

/* Ensure the cache can hold values up to index n */
static void ensure_capacity(idx_t n)
{
    size_t need = (size_t)(n + 1);
    if (need < 3) need = 3;           /* space for a(0) … a(2) */

    if (cache_cap >= need) return;

    seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
    if (tmp == NULL) exit(EXIT_FAILURE);
    cache = tmp;
    cache_cap = need;
}

/* Initialise the base cases a(0), a(1), a(2) */
static void initialise_base(void)
{
    cache[0] = 0;
    cache[1] = 0;
    cache[2] = 2;
    computed_to = 2;
}

/* Compute all a(i) up to and including n using the recurrence. */
static void compute_up_to(idx_t n)
{
    if (n <= computed_to) return;

    if (computed_to == 0) initialise_base();

    idx_t start = computed_to + 1;
    if (start < (idx_t)3) start = 3;   /* recurrence valid for n ≥ 3 */

    for (idx_t i = start; i <= n; ++i) {
        seq_t a_nm1 = cache[i - 1];
        seq_t a_nm2 = cache[i - 2];

        /* constants that fit in 64‑bit */
        uint64_t c1 = 2ull * (i - 1) * i;           // 2*(i-1)*i
        uint64_t c2 = (i - 1) * (i - 1) * i;        // (i-1)^2 * i

        /* Use a wider intermediate type when seq_t is small to avoid overflow. */
        if (sizeof(seq_t) <= 8) {
            /* 128‑bit intermediate for 64‑bit seq_t */
            __uint128_t t1 = (__uint128_t)a_nm1 * c1;
            __uint128_t t2 = (__uint128_t)a_nm2 * c2;
            __uint128_t num = t1 >= t2 ? t1 - t2 : t2 - t1;  // numerator
            cache[i] = (seq_t)(num / (i - 2));
        } else {
            /* seq_t is large enough to hold the intermediate product */
            seq_t t1 = a_nm1 * (seq_t)c1;
            seq_t t2 = a_nm2 * (seq_t)c2;
            seq_t num = t1 >= t2 ? t1 - t2 : t2 - t1;         // numerator
            cache[i] = num / (i - 2);
        }
    }

    computed_to = n;
}

/* Public entry point as required by the specification */
seq_t a(idx_t n)
{
    ensure_capacity(n);
    compute_up_to(n);
    return cache[n];
}
