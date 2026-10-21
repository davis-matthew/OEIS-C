/*
 * A248104  Positions of 0,1,0 in the Thue-Morse sequence (A010060).
 * https://oeis.org/A248104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Positions of 0,1,0 in the Thue–Morse sequence (A248104).
 * This implementation searches the Thue–Morse sequence for the
 * pattern 0,1,0 and memoises the starting positions. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* static cache of found positions */
static seq_t *pos_cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* next Thue–Morse index (1‑based) to examine */
static idx_t next_i = 1;

/* circular buffer holding the last three Thue–Morse bits,
   indexed by position modulo 3 (0,1,2). */
static seq_t buf[3] = {0, 0, 0};

/* compute the parity (Thue–Morse value) of x, returned as 0 or 1 */
static inline seq_t parity(idx_t x)
{
    seq_t p = 0;
    while (x) {
        p ^= 1;
        x &= x - 1;
    }
    return p;
}

/* ensure the cache has room for at least 'need' elements */
static void grow_cache(size_t need)
{
    if (need > cache_cap) {
        size_t newcap = cache_cap ? cache_cap * 2 : 64;
        if (newcap < need) newcap = need;
        seq_t *tmp = (seq_t *)malloc(newcap * sizeof(seq_t));
        if (!tmp) abort();
        if (pos_cache) {
            memcpy(tmp, pos_cache, cache_len * sizeof(seq_t));
            free(pos_cache);
        }
        pos_cache = tmp;
        cache_cap = newcap;
    }
}

/* generate position entries until at least 'target' terms are stored */
static void generate_until(idx_t target)
{
    while (cache_len < target) {
        /* current Thue–Morse bit (1‑based position next_i) */
        seq_t cur = parity(next_i - 1);

        /* test for pattern 0,1,0 that ends at position next_i */
        if (next_i >= 3) {
            idx_t a_idx = (next_i - 2) % 3;
            idx_t b_idx = (next_i - 1) % 3;
            seq_t a = buf[a_idx];
            seq_t b = buf[b_idx];
            if (a == 0 && b == 1 && cur == 0) {
                seq_t pos = (seq_t)(next_i - 2);        /* start position (1‑based) */
                grow_cache(cache_len + 1);
                pos_cache[cache_len++] = pos;
            }
        }

        /* store current bit for next windows */
        idx_t cur_idx = next_i % 3;
        buf[cur_idx] = cur;

        ++next_i;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* out of range: offset is 1 */
    generate_until(n);
    return pos_cache[n - 1];
}
