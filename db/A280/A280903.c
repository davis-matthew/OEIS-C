/*
 * A280903  Number of 2Xn 0..2 arrays with no element equal to more than one
 *          of its horizontal and antidiagonal neighbors, with the exception
 *          of exactly two elements, and with new values introduced in order 0
 *          sequentially upwards.
 * https://oeis.org/A280903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined by a 15‑term linear recurrence
 *            a(n)=15*a(n-1)-57*a(n-2)-88*a(n-3)+651*a(n-4)+333*a(n-5)
 *            -2520*a(n-6)-939*a(n-7)+4569*a(n-8)+1456*a(n-9)
 *            -4131*a(n-10)-933*a(n-11)+1793*a(n-12)+60*a(n-13)
 *            -336*a(n-14)+64*a(n-15) for n>17 Initial values (n=1..17) are
 *            hard‑coded from the OEIS entry
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Forward declaration of the function */
seq_t a(idx_t n);

/* Static cache for memoisation */
static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Ensure the cache can hold indices up to n */
static void ensure_cache(idx_t n)
{
    if (n+1 <= cache_cap) return;
    size_t need = n+1;
    size_t new_cap = cache_cap? cache_cap*2 : 512;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t*)realloc(cache, (new_cap+1)*sizeof(seq_t));
    if (!tmp) {
        /* In a real program you might abort or handle the error */
        abort();
    }
    cache = tmp;
    /* initialise new slots to 0 (should be unnecessary for first time) */
    for (size_t i = cache_cap+1; i <= need; ++i) cache[i] = 0;
    cache_cap = new_cap;
}

/* Initialise the first 17 terms */
static void init_initial_terms(void)
{
    static bool init = false;
    if (init) return;
    init = true;
    /* a(1) to a(17) */
    const seq_t init_vals[18] = {
        0,                  /* a(0) unused */
        0,                  /* a(1) */
        1,                  /* a(2) */
        10,                  /* a(3) */
        213,                 /* a(4) */
        2292,                /* a(5) */
        21762,               /* a(6) */
        184076,              /* a(7) */
        1457827,             /* a(8) */
        11000888,            /* a(9) */
        80107879,            /* a(10) */
        567420546,           /* a(11) */
        3931417538,          /* a(12) */
        26752272278,         /* a(13) */
        179329108005,        /* a(14) */
        1186930999936,       /* a(15) */
        7770945222145,       /* a(16) */
        50399529101088      /* a(17) */
    };
    ensure_cache(17);
    for (size_t i = 1; i <= 17; ++i) {
        cache[i] = init_vals[i];
    }
}

seq_t a(idx_t n)
{
    /* n >= 1, no bounds handling for n < 1 is required */
    if (n < 1) return 0;
    init_initial_terms();
    ensure_cache(n);

    if (cache[n] != 0 || n <= 17) {
        /* value already computed (or is an initial term) */
        return cache[n];
    }

    /* compute successive values up to n using the recurrence */
    for (idx_t i = 18; i <= n; ++i) {
        seq_t val = 15 * cache[i-1];
        val -= 57 * cache[i-2];
        val -= 88 * cache[i-3];
        val += 651 * cache[i-4];
        val += 333 * cache[i-5];
        val -= 2520 * cache[i-6];
        val -= 939 * cache[i-7];
        val += 4569 * cache[i-8];
        val += 1456 * cache[i-9];
        val -= 4131 * cache[i-10];
        val -= 933 * cache[i-11];
        val += 1793 * cache[i-12];
        val += 60 * cache[i-13];
        val -= 336 * cache[i-14];
        val += 64 * cache[i-15];
        cache[i] = val;
    }
    return cache[n];
}
