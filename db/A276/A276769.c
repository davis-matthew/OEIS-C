/*
 * A276769  Next larger number having the same digits as n.
 * https://oeis.org/A276769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, look
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 6665, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A276769 – Next larger number having the same digit set as n.
    The implementation scans upward from n+1 until it finds a number
    with exactly the same set of decimal digits as n (digits may be
    repeated arbitrarily).  Results are memoised so that repeated or
    out‑of‑order calls are fast.  The function works for any seq_t that
    supports the usual integer operators and for any idx_t that fits
    the number of requested terms.  The algorithm is O(d) per new term,
    where d is the distance to the next term.  */

/* #include "../utils/prime.h"  is not required – we use only standard headers. */
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* compute bit mask of decimal digits of x (0‑9)   */
static uint16_t digit_mask(seq_t x)
{
    uint16_t mask = 0;
    if (x == 0) return 1 << 0;   /* digit 0 */
    while (x != 0) {
        uint8_t d = (uint8_t)(x % 10);
        mask |= 1 << d;
        x /= 10;
    }
    return mask;
}

/* memoised cache for values a(n) up to the largest n requested so far   */
static seq_t *cache_vals  = NULL;
static uint8_t *cache_done = NULL;
static size_t   cache_cap  = 0;

/* grow cache to at least new_cap  */
static void ensure_cache(size_t new_cap)
{
    if (new_cap <= cache_cap) return;
    size_t new_size = new_cap > cache_cap * 2 ? new_cap : cache_cap * 2;
    if (new_size == 0) new_size = 16;
    cache_vals  = (seq_t *)   realloc(cache_vals,  new_size * sizeof(seq_t));
    cache_done = (uint8_t *) realloc(cache_done, new_size * sizeof(uint8_t));
    if (!cache_vals || !cache_done) { /* allocation failed – use abort for safety */
        abort();
    }
    for (size_t i = cache_cap; i < new_size; ++i) cache_done[i] = 0;
    cache_cap = new_size;
}

/* public function  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset 1 – defined only for n>=1 */
    if (n >= (idx_t)cache_cap)
        ensure_cache((size_t)n + 1);

    if (cache_done[n]) return cache_vals[n];

    uint16_t target_mask = digit_mask((seq_t)n);
    seq_t candidate = (seq_t)n + 1;

    while (1) {
        if (digit_mask(candidate) == target_mask) {
            cache_vals[n] = candidate;
            cache_done[n] = 1;
            return candidate;
        }
        candidate++;
    }
}
