/*
 * A296135  {0->01}-transform of the Fibonacci word A003849.
 * https://oeis.org/A296135
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n+1)*α' + ρ') - floor(n*α' + ρ') where α' =
 *            (sqrt(5)-1)/2, ρ' = sqrt(5)-2
 * Range:     u8 n <= 150, u16 n <= 150, u32 n <= 150, u64 n <= 150,
 *            u128 n <= 150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;      /* cache of computed sequence values */
static idx_t cache_len = 0;      /* number of values stored in cache */

static void build_cache(idx_t n)
{
    idx_t target = n + 1;        /* number of values required */
    /* free previous cache */
    if (cache) free(cache);
    cache = (seq_t *)malloc(target * sizeof(seq_t));
    cache_len = target;

    /* Temporary buffers to build the Fibonacci‑like concatenations */
    seq_t *seq_prevprev = NULL;  /* corresponds to A_k-2 */
    seq_t *seq_prev    = NULL;   /* corresponds to A_k-1 */
    size_t len_prevprev = 0;
    size_t len_prev     = 0;

    /* Base case k = 0 : A_0 = "01"   (length 2) */
    seq_prevprev = (seq_t *)malloc(2 * sizeof(seq_t));
    seq_prevprev[0] = 0;
    seq_prevprev[1] = 1;
    len_prevprev = 2;
    if (target <= 2) {
        memcpy(cache, seq_prevprev, target * sizeof(seq_t));
        free(seq_prevprev);
        return;
    }

    /* Base case k = 1 : A_1 = "011"  (length 3) */
    seq_prev = (seq_t *)malloc(3 * sizeof(seq_t));
    seq_prev[0] = 0;
    seq_prev[1] = 1;
    seq_prev[2] = 1;
    len_prev = 3;
    if (target <= 3) {
        memcpy(cache, seq_prev, target * sizeof(seq_t));
        free(seq_prevprev);
        free(seq_prev);
        return;
    }

    /* Build successive sequences A_k = A_(k-1) | A_(k-2) until enough length */
    while (1) {
        size_t len_new = len_prev + len_prevprev;
        seq_t *seq_new = (seq_t *)malloc(len_new * sizeof(seq_t));

        memcpy(seq_new, seq_prev, len_prev * sizeof(seq_t));
        memcpy(seq_new + len_prev,
               seq_prevprev, len_prevprev * sizeof(seq_t));

        /* recycle old buffers */
        free(seq_prevprev);
        seq_prevprev = seq_prev;
        seq_prev    = seq_new;
        len_prevprev = len_prev;
        len_prev     = len_new;

        if (target <= len_new) {
            memcpy(cache, seq_prev, target * sizeof(seq_t));
            /* free temporary buffers */
            free(seq_prevprev);
            free(seq_prev);
            break;
        }
    }
}

seq_t a(idx_t n)
{
    if (n >= cache_len) build_cache(n);
    return cache[n];
}
