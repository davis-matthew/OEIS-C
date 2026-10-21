/*
 * A297110  Xor-Moebius transform of A006068, inverse of the binary Gray code.
 * https://oeis.org/A297110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 9901, u32 n <= 9901, u64 n <= 9901,
 *            u128 n <= 9901, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Xor‑Moebius transform of A006068 (inverse of binary Gray code) */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Gray code inverse: m xor (m>>1) == n  ⇒  m = gray_inverse(n) */
static seq_t gray_inverse(seq_t g)
{
    seq_t r = 0U;
    while (g) {
        r ^= g;
        g >>= 1U;
    }
    return r;
}

/* -------------------------------------------------- cache for a(n) ------------ */
static seq_t *cache   = NULL;   /* values of a(n)  (cache[n])          */
static uint8_t *done   = NULL;   /* 1 if cache[n] is computed, else 0  */
static size_t   cap    = 0;      /* allocated size of the two arrays   */

static void ensure_cache(idx_t n)
{
    if ((size_t)n >= cap) {
        size_t new_cap = n + 1;
        seq_t   *tmp   = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        uint8_t *tmpd  = (uint8_t *)realloc(done, new_cap);
        if (!tmpd) abort();
        done = tmpd;
        for (size_t i = cap; i < new_cap; ++i) {
            cache[i] = 0;
            done[i]  = 0;
        }
        cap = new_cap;
    }
}

/* -------------------------------------------------- sequence a(n) -------------- */
seq_t a(idx_t n)
{
    if (n == 0U) return 0;   /* safe guard – offset 1 is the first value */

    ensure_cache(n);

    if (done[n]) return cache[n];

    /* compute the contribution from all proper divisors d < n */
    seq_t xor_div = 0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            /* divisor d, complementary divisor n/d */
            idx_t d1 = d;
            idx_t d2 = n / d;

            if (d1 < n) {
                if (!done[d1]) a(d1);
                xor_div ^= cache[d1];
            }
            if (d2 != d1 && d2 < n) {
                if (!done[d2]) a(d2);
                xor_div ^= cache[d2];
            }
        }
    }

    seq_t result = gray_inverse((seq_t)n) ^ xor_div;
    cache[n] = result;
    done[n]  = 1;
    return result;
}
