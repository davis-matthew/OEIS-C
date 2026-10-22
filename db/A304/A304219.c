/*
 * A304219  Number of n X 6 0..1 arrays with every element unequal to 0, 1, 3
 *          or 8 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A304219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + 5*a(n-3) + 2*a(n-4) - 3*a(n-5) -
 *            4*a(n-6) - 3*a(n-7) - 2*a(n-8) + 3*a(n-9) + 3*a(n-10) -
 *            3*a(n-11) - a(n-12) + a(n-13) + a(n-14) (for n > 19), with
 *            a(1)=13, a(2)=49, a(3)=56, a(4)=108, a(5)=279, a(6)=740,
 *            a(7)=1628, a(8)=3759, a(9)=9029, a(10)=20849, a(11)=48072,
 *            a(12)=11...
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 24, u64 n <= 50, u128 n <= 103,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;          /* cached sequence values */
static size_t cache_cap = 0;        /* allocated capacity (in seq_t elements) */
static idx_t cache_max = 0;          /* largest n for which cache[n] is valid */

/* Initial values for n = 1 .. 19 (index 0 unused) */
static const seq_t init_vals[20] = {
    0,          /* unused */
    13, 49, 56, 108, 279, 740, 1628, 3759, 9029, 20849,
    48072, 112793, 263016, 610412, 1423125, 3318263, 7722039, 17983738, 41903265
};

/* Ensure cache has room for indices up to `n`.  Extend by doubling capacity. */
static void ensure_cache_capacity(idx_t n)
{
    if (cache == NULL) {
        cache_cap = 32;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return;            /* allocation failure → future accesses will fail */
        cache[0] = 0;                  /* unused but harmless */
    }
    while (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return;              /* realloc failed → leave old cache intact */
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined for n < 1; return 0 for safety */

    ensure_cache_capacity(n);
    if (cache == NULL) return 0;   /* allocation failed */

    if (cache_max >= n) return cache[n];

    /* Compute values up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        if (i <= 19) {
            cache[i] = init_vals[i];
        } else {
            /* Sum of positive coefficients */
            seq_t sum_pos = cache[i-1];
            sum_pos += cache[i-2];
            sum_pos += (seq_t)5 * cache[i-3];
            sum_pos += (seq_t)2 * cache[i-4];
            sum_pos += (seq_t)3 * cache[i-9];
            sum_pos += (seq_t)3 * cache[i-10];
            sum_pos += cache[i-13];
            sum_pos += cache[i-14];

            /* Sum of negative coefficients */
            seq_t sum_neg = (seq_t)3 * cache[i-5];
            sum_neg += (seq_t)4 * cache[i-6];
            sum_neg += (seq_t)3 * cache[i-7];
            sum_neg += (seq_t)2 * cache[i-8];
            sum_neg += (seq_t)3 * cache[i-11];
            sum_neg += cache[i-12];

            cache[i] = sum_pos - sum_neg;   /* unsigned subtraction is safe as sum_pos ≥ sum_neg */
        }
    }

    cache_max = n;
    return cache[n];
}
