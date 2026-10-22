/*
 * A329911  The number of rooted chains of reflexive matrices of order n.
 * https://oeis.org/A329911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000629(n^2-n) = 2 * B_{n^2-n} for n>0, a(0)=1 where B_m
 *            is the ordered Bell (Fubini) number: B_m = sum_{k=0}^m S(m,k) *
 *            k! (S = Stirling 2nd kind). A binomial transform identity gives:
 *            sum_{k=0}^m C(m,k) * B_k = 2 * B_m (valid for m>0)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

static seq_t *B_cache = NULL;      // dynamic array of B numbers
static idx_t B_cache_size = 0;     // number of B values actually stored
static idx_t B_cache_cap  = 0;     // allocated capacity

/* Ensure B_cache holds entries up to index 'need' (inclusive) */
static void ensure_B_cache(idx_t need)
{
    if (B_cache_size > need) return;          // already computed
    /* Grow capacity if required */
    if (B_cache_cap < need + 1) {
        idx_t new_cap = B_cache_cap ? B_cache_cap * 2 : 16;
        if (new_cap < need + 1) new_cap = need + 1;
        seq_t *new_buf = (seq_t *)realloc(B_cache, new_cap * sizeof(seq_t));
        B_cache = new_buf;
        B_cache_cap = new_cap;
    }
    /* Initialize B[0] if not done yet */
    if (B_cache_size == 0) {
        B_cache[0] = 1;          /* B_0 = 1 */
        B_cache_size = 1;
    }
    /* Compute missing B_i values */
    for (idx_t i = B_cache_size; i <= need; ++i) {
        seq_t sum = 0;
        for (idx_t k = 0; k < i; ++k) {
            sum += binomial(i, k) * B_cache[k];
        }
        B_cache[i] = sum;
    }
    B_cache_size = need + 1;
}

/* Public function to compute a(n) */
seq_t a(idx_t n)
{
    /* m = n^2 - n, fits in idx_t for the maximum n considered */
    idx_t m = n*n - n;          /* n^2 - n */
    if (m == 0) return 1;       /* a(0)=a(1)=1 because m=0 */

    /* Ensure B_cache contains B_m */
    ensure_B_cache(m);

    /* Result is 2 * B_m for m>0 */
    seq_t result = B_cache[m];
    result += result;           /* multiply by 2 */
    return result;
}
