/*
 * A395817  Strictly increasing minimal sequence where consecutive terms can
 *          be added digit-by-digit without carrying in base 10.
 * https://oeis.org/A395817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 92, u16 n <= 3905, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_seq_array, grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A395817 – strictly increasing minimal sequence where each
 * consecutive pair has no digit-wise carry in base 10.
 *
 * Recurrence:
 *   a(1) = 1
 *   a(n) = min{ x > a(n-1) | d_i(x) + d_i(a(n-1)) <= 9 for all i }
 *
 * The algorithm finds the next term by examining the decimal digits of
 * a(n-1).  Let D[i] be the i‑th decimal digit of the previous term,
 * numbered from 0 (least significant).  For every position i the digit
 * of the next term X[i] must satisfy X[i] <= 9 - D[i].
 *
 * Minimality is achieved by choosing the least significant position
 * that can be increased: we scan from most significant to least
 * significant, keeping track of whether a more significant digit is
 * >4 (i.e. requires X to be strictly smaller).  The rightmost digit
 * that can be increased while all more significant digits are <=4
 * becomes the increment position.  The digits below it are set to 0
 * (minimal value).  If no such position exists, the next term is
 * 10^k where k is the number of digits of a(n‑1). */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/grow_seq.h"

static seq_t *cache = NULL;
static idx_t  cache_len = 0;
static size_t cache_cap = 0;

/* Return the next term after prev according to the definition. */
static seq_t next_value(seq_t prev)
{
    /* Extract decimal digits of prev into buf (LSB first). */
    uint8_t buf[128];
    size_t len = 0;
    seq_t t = prev;
    if (t == 0) {
        buf[0] = 0;
        len = 1;
    } else {
        while (t > 0) {
            buf[len++] = (uint8_t)(t % 10);
            t /= 10;
        }
    }

    /* Find the least significant position that can be increased. */
    int candidate = -1;
    bool seen_gt4 = false;
    for (int i = (int)len - 1; i >= 0; --i) {
        if (buf[i] > 4) {
            seen_gt4 = true;
        } else if (!seen_gt4) {
            candidate = i;          /* keep searching for a less significant one */
        }
    }

    if (candidate == -1) {
        /* All digits >4 – next term is 10^len. */
        seq_t p = 1;
        for (size_t i = 0; i < len; ++i) p *= 10;
        return p;
    }

    /* Increment the chosen digit and zero out lower ones. */
    buf[candidate] += 1;
    for (int j = 0; j < candidate; ++j) buf[j] = 0;

    /* Reconstruct the number from digits. */
    seq_t res = 0;
    for (int i = (int)len - 1; i >= 0; --i) {
        res = res * 10 + buf[i];
    }
    return res;
}

/* Ensure cache has capacity for at least 'size' terms. */
static void ensure_capacity(idx_t size)
{
    if ((idx_t)cache_cap >= size) return;
    if (!cache) {
        cache = alloc_seq_array((size_t)size);
        cache_cap = (size_t)size;
    } else {
        grow_seq(&cache, &cache_cap, (size_t)size);
    }
}

/* Required sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined for n=0 */

    if (cache_len == 0) {
        ensure_capacity(1);
        cache[0] = (seq_t)1;          /* a(1) */
        cache_len = 1;
    }

    while (cache_len < n) {
        seq_t prev = cache[cache_len - 1];
        seq_t nxt  = next_value(prev);

        if ((size_t)cache_len == cache_cap)
            grow_seq(&cache, &cache_cap, (size_t)cache_len + 1);

        cache[cache_len++] = nxt;
    }
    return cache[n - 1];
}
