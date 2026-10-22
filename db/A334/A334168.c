/*
 * A334168  Numbers m whose leading digit and the other decimal digits appear
 *          respectively before and directly after the decimal point of its
 *          m-th root.
 * https://oeis.org/A334168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 10^(n-1) + floor((n-1)*ln(10))
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   ensure_cache, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mul_small.h"   /* seq_t mul_small(seq_t x, unsigned int m) */
#include "../utils/ensure_cache.h" /* void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) */

seq_t a(idx_t n)
{
    if (n == 0)          /* safety for out‑of‑bounds calls */
        return (seq_t)0;

    /* --- 10^(n‑1) ----------------------------------------------------- */
    static seq_t *pow10_cache      = NULL; /* pow10_cache[i] = 10^i,  i = 0..() */
    static size_t pow10_cache_len  = 0;    /* number of computed entries */
    static size_t pow10_cache_cap  = 0;    /* allocated capacity */

    /* request at least n elements (we need index up to n‑1) */
    ensure_cache(n, &pow10_cache, &pow10_cache_cap);

    if (pow10_cache_len == 0) {
        /* initialise first entry 10^0 = 1 */
        pow10_cache[0] = (seq_t)1;
        pow10_cache_len = 1;
    }

    /* compute missing powers of ten up to n‑1 */
    for (idx_t i = pow10_cache_len; i < n; ++i)
        pow10_cache[i] = mul_small(pow10_cache[i - 1], 10);
    pow10_cache_len = n;

    seq_t pow10k = pow10_cache[n - 1];   /* 10^(n‑1) */

    /* --- floor((n‑1)*ln(10)) ------------------------------------------ */
    const uint64_t LOG10_NUM = 2302585092994046181ULL; /* ln(10) * 10^18 (rounded) */
    const uint64_t LOG10_DEN = 1000000000000000000ULL; /* 10^18 */

    idx_t k = n - 1;   /* coefficient for ln(10) */

    unsigned __int128 prod = (unsigned __int128)k * LOG10_NUM;
    idx_t extra = (idx_t)(prod / LOG10_DEN);

    /* --- final result --------------------------------------------------- */
    return pow10k + (seq_t)extra;
}
