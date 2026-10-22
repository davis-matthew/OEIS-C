/*
 * A352380  Numbers k such that no nonzero digit of 3*k divides 3*k.
 * https://oeis.org/A352380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 1530, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the n-th integer k such that no non‑zero digit of 3·k divides 3·k */

#include <stdlib.h>
#include <stdbool.h>

/* true if k satisfies the condition: 3*k is not divisible by any of its non‑zero decimal digits */
static bool good(seq_t k)
{
    seq_t m = k * (seq_t)3;      /* 3·k */
    seq_t t = m;
    while (t > 0) {
        unsigned int d = (unsigned int)(t % 10);  /* current decimal digit */
        if (d != 0 && (m % d) == 0)              /* digit divides 3·k? */
            return false;
        t = t / 10;
    }
    return true;
}

/* cache of previously found terms */
static seq_t *cache    = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static seq_t next_k = (seq_t)1;   /* candidate k to test next */

/* enlarge cache to at least `need` entries */
static void ensure_cap(size_t need)
{
    if (cache_cap >= need)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                 /* unrecoverable allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)           /* undefined for 0, return 0 as safety */
        return (seq_t)0;

    /* fill cache until it contains at least n terms */
    while (cache_len < (size_t)n) {
        ensure_cap(cache_len + 1);
        if (good(next_k))
            cache[cache_len++] = next_k;
        next_k = next_k + 1;
    }
    return cache[n - 1];
}
