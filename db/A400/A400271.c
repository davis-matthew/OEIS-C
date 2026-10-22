/*
 * A400271  G.f. A(x) satisfies A(x) = (1 + x) * A(x^4 * (1 + x)), with A(0) =
 *          1.
 * https://oeis.org/A400271
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..⌊n/4⌋} binomial(k+1, n-4k) * a(k), a(0)=1
 * Range:     u8 n <= 65, u16 n <= 69, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

static seq_t  *cache   = NULL;   /* memoised sequence values   */
static unsigned char *done = NULL;/* 1 if cache[n] valid, else 0 */
static size_t cap = 0;           /* current allocated size     */

/* Ensure that cache and done have room for index n   */
static void
ensure_arrays(idx_t n)
{
    if ((size_t)n < cap) return;
    size_t new_cap = (cap == 0) ? 16 : cap * 2;
    while (new_cap <= (size_t)n) new_cap *= 2;

    seq_t   *tmp_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    unsigned char *tmp_done = (unsigned char*)realloc(done, new_cap * sizeof(unsigned char));
    if (!tmp_cache || !tmp_done) abort();          /* allocation failure */

    cache = tmp_cache;
    done  = tmp_done;
    for (size_t i = cap; i < new_cap; ++i) done[i] = 0; /* mark uncomputed */
    cap = new_cap;
}

seq_t a(idx_t n)
{
    ensure_arrays(n);

    if (done[n]) return cache[n];   /* found in cache */

    seq_t sum = 0;
    if (n == 0) {
        sum = 1;
    } else {
        idx_t kmax = n / 4;
        for (idx_t k = 0; k <= kmax; ++k) {
            idx_t m = n - 4 * k;                 /* choose argument for binomial */
            if (m > k + 1) continue;            /* binomial would be zero */
            seq_t coeff = binomial(k + 1, m);
            if (coeff == 0) continue;
            seq_t ak = a(k);                    /* recursion with memoisation */
            sum += coeff * ak;                  /* multiply and add */
        }
    }

    cache[n] = sum;
    done[n] = 1;
    return sum;
}
