/*
 * A355722  Row 2 of table A355721.
 * https://oeis.org/A355722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ( (2n+3)!! / 3 ) - Σ_{i=0}^{n-1} a(i) * (2(n-i)+1)!!,
 *            a(0) = 1. The recurrence follows from the generating function
 *            A(x) = ( Σ_{k≥0} d(k+2)/d(2) x^k ) / ( Σ_{k≥0} d(k+1)/d(1) x^k
 *            ), where d(m) = ∏_{k=1..m} (2k-1) = (2m-1)!!
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static caches for memoisation */
    static seq_t *a_cache = NULL;
    static seq_t *b_cache = NULL;
    static size_t a_cap  = 0;
    static size_t b_cap  = 0;
    static idx_t  a_max  = (idx_t)-1;   /* highest index in a_cache that is set   */
    static idx_t  b_max  = (idx_t)-1;   /* highest index in b_cache that is set   */

    /* Ensure capacity for A(n) entries */
    if (a_cache == NULL) {
        a_cap = n + 1;
        if (a_cap < 16) a_cap = 16;
        a_cache = (seq_t *)malloc(sizeof(seq_t) * a_cap);
        if (a_cache == NULL) abort();          /* in production we would handle this */
        a_max = (idx_t)-1;                     /* not yet set */
    } else if (a_cap < (size_t)(n + 1)) {
        size_t new_cap = a_cap;
        while (new_cap < (size_t)(n + 1)) new_cap <<= 1;
        a_cache = (seq_t *)realloc(a_cache, sizeof(seq_t) * new_cap);
        if (a_cache == NULL) abort();
        a_cap = new_cap;
    }

    /* Ensure capacity for B(n+1) entries (since we need b[n+1]) */
    if (b_cache == NULL) {
        b_cap = n + 2;
        if (b_cap < 16) b_cap = 16;
        b_cache = (seq_t *)malloc(sizeof(seq_t) * b_cap);
        if (b_cache == NULL) abort();
        b_max = (idx_t)-1;
    } else if (b_cap < (size_t)(n + 2)) {
        size_t new_cap = b_cap;
        while (new_cap < (size_t)(n + 2)) new_cap <<= 1;
        b_cache = (seq_t *)realloc(b_cache, sizeof(seq_t) * new_cap);
        if (b_cache == NULL) abort();
        b_cap = new_cap;
    }

    /* Initialise b[0] if first time */
    if (b_max == (idx_t)-1) {
        b_cache[0] = (seq_t)1;   /* (2*0+1)!! = 1 */
        b_max = 0;
    }

    /* Compute b up to n+1 (double‑factorials) */
    for (idx_t k = b_max + 1; k <= n + 1; ++k) {
        seq_t mult = (seq_t)(2 * k + 1);
        b_cache[k] = b_cache[k - 1] * mult;
    }
    b_max = n + 1;

    /* Initialise a[0] if first time */
    if (a_max == (idx_t)-1) {
        a_cache[0] = (seq_t)1;
        a_max = 0;
    }

    /* Compute a(k) for k = a_max+1 .. n */
    for (idx_t k = a_max + 1; k <= n; ++k) {
        seq_t sum = 0;
        /* sum_{i=0}^{k-1} a(i) * b(k-i) */
        for (idx_t i = 0; i < k; ++i) {
            sum += a_cache[i] * b_cache[k - i];
        }
        /* c(k) = (2k+3)!! / 3 = b(k+1) / 3 */
        seq_t c = b_cache[k + 1] / (seq_t)3;
        a_cache[k] = c - sum;
    }
    a_max = n;

    return a_cache[n];
}
