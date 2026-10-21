/*
 * A288002  L-fusc, sequence l of the mutual diatomic recurrence pair: l(1)=0,
 *          r(1)=1, l(2n) = l(n), r(2n) = r(n), l(2n+1) = l(n)+r(n), r(2n+1) =
 *          l(n+1)+r(n+1), where r(n) = A288003(n).
 * https://oeis.org/A288002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = l-fusc(n), computed by the mutually recursive definitions
 *            l(1) = 0, r(1) = 1 l(2n) = l(n), r(2n) = r(n) l(2n+1) =
 *            l(n)+r(n), r(2n+1) = l(n+1)+r(n+1) a(n) is defined to be l(n)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Forward declarations of the recursive helpers */
static seq_t get_l(idx_t n);
static seq_t get_r(idx_t n);

/* Memoisation cache for l(n) */
static seq_t *l_cache = NULL;
static unsigned char *l_done = NULL;
static idx_t l_cache_size = 0;

/* Memoisation cache for r(n) */
static seq_t *r_cache = NULL;
static unsigned char *r_done = NULL;
static idx_t r_cache_size = 0;

/* Ensure the cache can hold indices up to n (inclusive) */
static void ensure_cache(idx_t n, seq_t **cache, unsigned char **done,
                         idx_t *size)
{
    if (*size > n) return;
    idx_t newSize = (*size == 0) ? 2 : (*size);
    while (newSize <= n) newSize <<= 1;
    *cache = (seq_t *)realloc(*cache, newSize * sizeof(seq_t));
    *done  = (unsigned char *)realloc(*done,  newSize * sizeof(unsigned char));
    for (idx_t i = *size; i < newSize; ++i) (*done)[i] = 0;
    *size = newSize;
}

/* Recursive function to compute l(n) with memoisation */
static seq_t get_l(idx_t n)
{
    ensure_cache(n, &l_cache, &l_done, &l_cache_size);
    if (l_done[n]) return l_cache[n];
    seq_t res;
    if (n == 1) {
        res = 0;
    } else if ((n & 1) == 0) {
        res = get_l(n >> 1);
    } else {
        idx_t k = (n - 1) >> 1;
        res = get_l(k) + get_r(k);
    }
    l_cache[n] = res;
    l_done[n]  = 1;
    return res;
}

/* Recursive function to compute r(n) with memoisation */
static seq_t get_r(idx_t n)
{
    ensure_cache(n, &r_cache, &r_done, &r_cache_size);
    if (r_done[n]) return r_cache[n];
    seq_t res;
    if (n == 1) {
        res = 1;
    } else if ((n & 1) == 0) {
        res = get_r(n >> 1);
    } else {
        idx_t k = (n - 1) >> 1;
        res = get_l(k + 1) + get_r(k + 1);
    }
    r_cache[n] = res;
    r_done[n]  = 1;
    return res;
}

/* Public function required by the build */
seq_t a(idx_t n)
{
    return get_l(n);
}
