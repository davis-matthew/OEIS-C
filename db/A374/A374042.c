/*
 * A374042  Numbers k such that A276085(k) and A328768(k) are both multiples
 *          of 3, where A276085 is the primorial base log-function, and
 *          A328768 is the first primorial based variant of arithmetic
 *          derivative.
 * https://oeis.org/A374042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer k such that ord_3(k) ≠ 1 and v2(k) ≡
 *            v3(k) (mod 3), where v2 and v3 are the 2‑adic and 3‑adic
 *            valuations
 * Range:     u8 n <= 105, u16 n <= 12000, u32 n <= 12000, u64 n <= 12000,
 *            u128 n <= 12000, big any n
 * Validated: standard, pass, 88 terms through n = 9889 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <assert.h>
#include <stdlib.h>

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

static seq_t *cache = NULL;        /* array of found terms          */
static size_t cache_size = 0;      /* number of terms currently cached */
static size_t cache_cap = 0;       /* allocated capacity of cache    */
static idx_t next_candidate = 1;   /* next integer to test           */

static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = need < 64 ? 64 : need * 2;
    cache = seq_realloc(cache, cache_cap, new_cap);
    if (!cache)
        abort();                   /* memory allocation failed   */
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    assert(n >= 1);

    if (cache_size < n) {
        ensure_cache_cap(n);
        while (cache_size < n) {
            seq_t cand = (seq_t)next_candidate;
            seq_t temp = cand;
            seq_t e2 = 0, e3 = 0;

            /* count factors of 2 */
            while ((temp & 1) == 0) {
                temp >>= 1;
                ++e2;
            }
            /* count factors of 3 */
            while (temp % 3 == 0) {
                temp /= 3;
                ++e3;
            }

            if (e3 != 1 && (e2 % 3) == (e3 % 3))
                cache[cache_size++] = cand;

            ++next_candidate;
        }
    }
    return cache[n - 1];
}
