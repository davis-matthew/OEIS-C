/*
 * A296692  Numbers whose base-3 digits d(m), d(m-1), ... d(0) have #(rises) >
 *          #(falls); see Comments.
 * https://oeis.org/A296692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A296692: numbers whose base‑3 digits have more rises than falls
   (reaches: a rise is i such that d(i) < d(i+1), d(i) is a more
   significant digit).  Linear scan with memoization. */

#include <stdlib.h>
#include <stdbool.h>

/* Return true if x has more rises than falls in its base‑3 representation. */
static bool qualifies(seq_t x)
{
    seq_t t = x;
    uint8_t digits[128];
    size_t len = 0;

    /* Extract base‑3 digits, least significant first. */
    while (t > 0) {
        digits[len++] = (uint8_t)(t % 3);
        t = t / 3;
    }

    if (len < 2)                  /* no adjacent pair */
        return false;

    int rises = 0, falls = 0;

    /* Compare digits from most significant to least significant.
       digits[len-1] is most significant. */
    for (size_t i = len; i-- > 1; ) {
        uint8_t hi = digits[i];
        uint8_t lo = digits[i - 1];
        if (hi < lo)
            rises++;
        else if (hi > lo)
            falls++;
    }

    return rises > falls;
}

/* Return the n‑th element of the sequence (offset 1). */
seq_t a(idx_t n)
{
    /* Handle the offset. */
    if (n == 0)
        return 0;

    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static seq_t next_candidate = 1;   /* number to test next */

    /* Return from cache if already computed. */
    if (cache_len >= n)
        return cache[n - 1];

    /* Lazy allocation of the cache array. */
    if (cache_cap == 0) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Find more qualifying numbers until the desired index is reached. */
    while (cache_len < n) {
        if (qualifies(next_candidate)) {
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = next_candidate;
        }
        next_candidate += 1;        /* advance to the next candidate */
    }

    return cache[n - 1];
}
