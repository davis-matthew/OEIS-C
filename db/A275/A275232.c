/*
 * A275232  Number of 7Xn 0..2 arrays with no element equal to any value at
 *          offset (-2,-1) (-2,1) or (-1,0) and new values introduced in order
 *          0..2.
 * https://oeis.org/A275232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 20*a(n-1) + 57*a(n-2) - 1206*a(n-3) + 3096*a(n-4) +
 *            2306*a(n-5) - 16957*a(n-6) + 20440*a(n-7) - 7755*a(n-8) for n >
 *            9 initial values: a(1)=32, a(2)=404, a(3)=8134, a(4)=164645,
 *            a(5)=3334854, a(6)=67550874, a(7)=1368326231, a(8)=27717164160,
 *            a(9)=561446064338
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/mul_small.h"

/* helper for casting the pointer returned by realloc in C++ */
#ifdef __cplusplus
#  define CAST_TO_SEQ_T(p) reinterpret_cast<seq_t*>(p)
#else
#  define CAST_TO_SEQ_T(p) ((seq_t*)(p))
#endif

/* static cache for previously computed values */
static seq_t *seq_cache = NULL;   /* array storing a(0)..a(max) */
static size_t cache_size = 0;     /* allocated size (in elements) */
static idx_t  cache_max  = 0;     /* highest n already computed */

/* initial terms (a(0) unused but convenient for 1–based indexing) */
static const seq_t init_vals[10] = {
    0ULL,                      /* a(0) */
    32ULL, 404ULL, 8134ULL, 164645ULL,
    3334854ULL, 67550874ULL, 1368326231ULL,
    27717164160ULL, 561446064338ULL
};

/* make sure values up to n are computed and cached */
static void ensure_cache(idx_t n)
{
    if (n <= cache_max) return;         /* already available */

    idx_t i;
    /* enlarge array if necessary */
    size_t new_sz = (size_t)(n + 1);     /* need indices 0 .. n */
    if (new_sz > cache_size) {
        void *tmp = realloc(seq_cache, new_sz * sizeof(seq_t));
        if (!tmp) abort();             /* allocation failure */
        seq_cache = CAST_TO_SEQ_T(tmp);
        cache_size = new_sz;
    }

    /* compute missing values */
    for (i = cache_max + 1; i <= n; ++i) {
        if (i <= 9) {
            seq_cache[i] = init_vals[i];
        } else {
            /* recurrence: a(n) = Σ coeff[k] * a(n-1-k) */
            const int coeff[8] = {
                20, 57, -1206, 3096,
                2306, -16957, 20440, -7755
            };
            seq_t sum_pos = 0;
            seq_t sum_neg = 0;
            int k;
            for (k = 0; k < 8; ++k) {
                seq_t prod = mul_small(seq_cache[i - 1 - k],
                                       (unsigned int)(abs(coeff[k])));
                if (coeff[k] > 0)
                    sum_pos += prod;
                else
                    sum_neg += prod;
            }
            seq_cache[i] = sum_pos - sum_neg;   /* sum_pos ≥ sum_neg by construction */
        }
    }
    cache_max = n;
}

/* public function returning a(n) */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* sequence is 1-indexed, undefined for n < 1 */
    ensure_cache(n);
    return seq_cache[n];
}
