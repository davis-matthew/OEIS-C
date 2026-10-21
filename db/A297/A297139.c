/*
 * A297139  Numbers whose base-7 digits d(m), d(m-1),..., d(0) have m=0 or
 *          else d(i) = d(i+1) for some i in {0,1,...,m-1}.
 * https://oeis.org/A297139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = the n‑th positive integer whose base‑7 representation
 *            contains at least one pair of equal adjacent digits, or is a
 *            single–digit number. (Thus a(1)=1, a(2)=2, …, a(24)=89 for the
 *            first 24 terms.)
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* Check whether a number belongs to the sequence. */
static bool in_sequence(seq_t val)
{
    /* One‑digit numbers are always in the sequence. */
    if (val < 7)
        return true;

    seq_t prev = val % 7;
    val /= 7;
    while (val > 0) {
        seq_t cur = val % 7;
        if (cur == prev)
            return true;
        prev = cur;
        val /= 7;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)                    /* undefined for n = 0 – not expected */
        return 0;

    static seq_t *cache      = NULL;   /* generated terms */
    static size_t cache_len = 0;       /* number of stored terms */
    static size_t cache_cap = 0;       /* allocated capacity */

    size_t need = (size_t)n;
    if (cache_len < need) {
        /* Grow the cache if necessary. */
        size_t new_cap = cache_cap ? cache_cap : 4;
        while (new_cap < need)
            new_cap <<= 1;
        if (new_cap > cache_cap) {
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)
                abort();              /* allocation failed */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Generate missing terms. */
        seq_t val = cache_len ? cache[cache_len - 1] + 1 : 1;
        while (cache_len < need) {
            if (in_sequence(val))
                cache[cache_len++] = val;
            ++val;
        }
    }

    return cache[need - 1];
}
