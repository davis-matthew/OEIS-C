/*
 * A397885  a(n) is the number of tilings of a 7 X 5n rectangle into
 *          I-pentominoes.
 * https://oeis.org/A397885
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 8*a(n-1) - 10*a(n-2) + 10*a(n-3) - 5*a(n-4) + a(n-5) (n>4),
 * a(0)=1, a(1)=4, a(2)=28, a(3)=190, a(4)=1276.
 */

#include <stdlib.h>

/* shared memoisation buffer */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* ensure space for indices up to n */
static void ensure_size(idx_t n)
{
    if ((idx_t)(cache_cap) > n)
        return;

    size_t needed = (size_t)n + 1;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < needed)
        new_cap <<= 1;

    /* C++ needs a cast from void* to seq_t* */
    seq_t *tmp = (seq_t*)realloc((void*)cache, new_cap * sizeof(seq_t));
    if (tmp == NULL) {
        /* allocation failed – unlikely for n < 10000, but abort */
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n < cache_len)
        return cache[n];

    ensure_size(n);

    /* initialise the first five terms once */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)28;
        cache[3] = (seq_t)190;
        cache[4] = (seq_t)1276;
        cache_len = 5;
    }

    while (cache_len <= n) {
        /* compute a[cache_len] using the recurrence */
        seq_t term1 = (seq_t)8  * cache[cache_len-1];
        seq_t term2 = (seq_t)10 * cache[cache_len-2];
        seq_t term3 = (seq_t)10 * cache[cache_len-3];
        seq_t term4 = (seq_t)5  * cache[cache_len-4];
        seq_t term5 = cache[cache_len-5];

        cache[cache_len] = term1 - term2 + term3 - term4 + term5;
        ++cache_len;
    }

    return cache[n];
}
