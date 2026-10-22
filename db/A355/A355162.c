/*
 * A355162  a(n) = exp(-1) * Sum_{k>=0} (4*k + 2)^n / k!.
 * https://oeis.org/A355162
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = 2*a(n-1) + Σ_{k=1..n} C(n-1,k-1)*4^k*a(n-k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm memoizes computed values and powers of 4.
   It works for all provided seq_t/idx_t types, including arbitrary‑precision
   integer classes.  */
#include <stdlib.h>

/* make sure an array of at least “needed” elements exists */
static void ensure_cap(idx_t needed, seq_t **arr, idx_t *cap)
{
    if (needed <= *cap) return;
    idx_t new_cap = (*cap) ? (*cap << 1) : 8;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    *arr = tmp;
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Memoised cache: a[i] contains a(i) for i ≤ last_computed */
    static seq_t *cache        = NULL;
    static idx_t cache_cap    = 0;
    static idx_t last_computed = 0; /* highest n computed so far (≥0) */

    /* Pre‑computed powers of 4: pow4[k] = 4^k */
    static seq_t *pow4          = NULL;
    static idx_t pow4_cap      = 0;
    static idx_t pow4_last     = 0; /* highest k computed so far */

    /* first call: initialise cache[0] = 1 */
    if (!cache) {
        ensure_cap(1, &cache, &cache_cap);
        cache[0] = 1;
    }

    if (n == 0) return cache[0];

    /* ensure powers of 4 up to n are available */
    if (n > pow4_last) {
        ensure_cap(n + 1, &pow4, &pow4_cap);
        if (pow4_last == 0) {
            pow4[0] = 1;
            pow4[1] = 4;
            pow4_last = 1;
        }
        for (idx_t k = pow4_last + 1; k <= n; ++k) {
            pow4[k] = pow4[k - 1] * (seq_t)4;
        }
        pow4_last = n;
    }

    /* compute any missing a(i) values up to n */
    if (n > last_computed) {
        ensure_cap(n + 1, &cache, &cache_cap);
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            /* term 2*a(i-1) */
            seq_t sum = (seq_t)2 * cache[i - 1];

            /* sum over k = 1..i of C(i-1,k-1)*4^k*a(i-k) */
            seq_t bin = 1; /* C(i-1,0) */
            for (idx_t k = 1; k <= i; ++k) {
                seq_t term = bin * pow4[k] * cache[i - k];
                sum += term;
                if (k < i) {
                    /* C(i-1,k) = C(i-1,k-1)*(i-k)/k, division is exact */
                    bin = bin * (seq_t)(i - k) / (seq_t)k;
                }
            }
            cache[i] = sum;
        }
        last_computed = n;
    }

    return cache[n];
}
