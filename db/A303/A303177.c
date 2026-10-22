/*
 * A303177  Number of nX3 0..1 arrays with every element equal to 0, 1, 2, 5
 *          or 6 horizontally, diagonally or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A303177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 4, a(2) = 17, a(3) = 31, a(4) = 135, a(5) = 547, a(6) =
 *            1938, a(7) = 6965, a(8) = 25953, a(9) = 95602, a(10) = 350544,
 *            a(11) = 1288829, a(12) = 4742540, a(13) = 17436564; For n > 13:
 *            a(n) = 2*a(n-1) + 3*a(n-2) + 12*a(n-3) + 4*a(n-4) - 14*a(n-5) -
 *            17*a(n-6) - 26*a(n-7) + 52*a(n-8) - 8*a(n-1...
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

static const seq_t init_terms[13] = {
    4ULL, 17ULL, 31ULL, 135ULL, 547ULL, 1938ULL, 6965ULL, 25953ULL, 95602ULL,
    350544ULL, 1288829ULL, 4742540ULL, 17436564ULL
};

/* coefficient array: coef[1] = coefficient of a(n-1), ... coef[13] = coefficient of a(n-13) */
static const int coef[14] = {
    0,    /* unused */
    2,    /* a(n-1)   */
    3,    /* a(n-2)   */
    12,   /* a(n-3)   */
    4,    /* a(n-4)   */
    -14,  /* a(n-5)   */
    -17,  /* a(n-6)   */
    -26,  /* a(n-7)   */
    52,   /* a(n-8)   */
    0,    /* a(n-9)   */
    -8,   /* a(n-10)  */
    0,    /* a(n-11)  */
    8,    /* a(n-12)  */
    -32   /* a(n-13)  */
};

/* Reallocate cache to hold indices up to new_size (inclusive). */
static void ensure_cache(idx_t new_size)
{
    if (new_size <= cache_size)
        return;
    size_t new_cap = (size_t)(new_size + 1);   /* indices 0..new_size */
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr) {
        /* fatal error: out of memory */
        exit(EXIT_FAILURE);
    }
    cache = new_ptr;
    /* initialise newly allocated entries to 0 */
    for (idx_t i = cache_size + 1; i <= new_size; ++i)
        cache[i] = (seq_t)0;
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;        /* undefined for n<1, return 0 */

    /* initialise cache with first 13 terms on first call */
    if (cache_size == 0) {
        ensure_cache(13);
        for (idx_t i = 1; i <= 13; ++i)
            cache[i] = init_terms[i - 1];
    }

    idx_t old_size = cache_size;   /* current highest computed index */
    if (n > old_size) {
        /* grow cache to at least n */
        ensure_cache(n);
        /* compute missing values from old_size+1 up to n */
        for (idx_t i = old_size + 1; i <= n; ++i) {
            seq_t sum = (seq_t)0;
            for (int s = 1; s <= 13; ++s) {
                int c = coef[s];
                if (c == 0)
                    continue;
                seq_t prev = cache[i - s];
                if (c > 0)
                    sum += (seq_t)c * prev;
                else
                    sum -= (seq_t)(-c) * prev;
            }
            cache[i] = sum;
        }
    }

    return cache[n];
}
