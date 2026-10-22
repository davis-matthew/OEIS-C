/*
 * A302665  Number of nX3 0..1 arrays with every element equal to 1, 2, 3 or 6
 *          horizontally, diagonally or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A302665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A302665
   Empirical recurrence (valid for n ≥ 15):
      a(n) = 4*a(n-1)+4*a(n-2)-11*a(n-3)-27*a(n-4)-12*a(n-5)
             +85*a(n-6)+22*a(n-7)-22*a(n-8)-108*a(n-9)
             +72*a(n-10)-80*a(n-11)+120*a(n-12)+64*a(n-14)
   Initial values:
      a(1) = 1,  a(2) = 14,  a(3) = 43,  a(4) = 194,  a(5) = 691,
      a(6) = 2802, a(7) = 10660, a(8) = 41839, a(9) = 161878,
      a(10) = 630842, a(11) = 2449576, a(12) = 9528949,
      a(13) = 37035686, a(14) = 144004769
*/

#include <stdlib.h>
#include <string.h>

/* store the first 14 terms */
static const seq_t init[14] = {
    (seq_t)1,      (seq_t)14,     (seq_t)43,     (seq_t)194,
    (seq_t)691,    (seq_t)2802,   (seq_t)10660,  (seq_t)41839,
    (seq_t)161878, (seq_t)630842, (seq_t)2449576,(seq_t)9528949,
    (seq_t)37035686,(seq_t)144004769
};

/* dynamic cache of previously computed terms */
static seq_t *cache = NULL;      /* array of a(k) for k = 1 .. cache_size */
static idx_t cache_size = 0;     /* number of terms currently stored */
static idx_t cache_cap  = 0;     /* allocated capacity of cache */

/* ensure that terms up to and including n are computed */
static void ensure_cache(idx_t n)
{
    if (n <= cache_size) return;          /* nothing to do */

    /* grow the cache if necessary */
    idx_t need = n;
    if (need < 14) need = 14;              /* we always store the first 14 */

    if (!cache) {
        cache = (seq_t *)malloc(need * sizeof(seq_t));
        if (!cache) return;                /* allocation failure – caller will crash */
        cache_cap = need;
    } else if (need > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (!tmp) return;                  /* allocation failure */
        cache = tmp;
        cache_cap = need;
    }

    /* copy the initial 14 terms if they haven't been copied yet */
    if (cache_size < 14) {
        memcpy(cache, init, 14 * sizeof(seq_t));
        if (cache_size < 14) cache_size = 14;
    }

    /* compute further terms using the empirical recurrence */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        idx_t idx = i - 1;                 /* zero‑based index of a(i) */

        seq_t val =
            ((seq_t)4) * cache[idx - 1] +
            ((seq_t)4) * cache[idx - 2] -
            ((seq_t)11) * cache[idx - 3] -
            ((seq_t)27) * cache[idx - 4] -
            ((seq_t)12) * cache[idx - 5] +
            ((seq_t)85) * cache[idx - 6] +
            ((seq_t)22) * cache[idx - 7] -
            ((seq_t)22) * cache[idx - 8] -
            ((seq_t)108) * cache[idx - 9] +
            ((seq_t)72) * cache[idx -10] -
            ((seq_t)80) * cache[idx -11] +
            ((seq_t)120) * cache[idx -12] +
            ((seq_t)64) * cache[idx -14];

        cache[idx] = val;
    }

    if (n > cache_size) cache_size = n;
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n<1 */
    ensure_cache(n);
    return cache[n - 1];
}
