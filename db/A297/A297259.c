/*
 * A297259  Numbers whose base-6 digits have equal down-variation and
 *          up-variation; see Comments.
 * https://oeis.org/A297259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = nth integer whose base‑6 digits have equal down‑variation
 *            and up‑variation
 * Range:     u8 n <= 47, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static bool equal_variation(seq_t x)
{
    if (x < 6) return true;           /* single digit numbers always qualify */

    uint8_t digits[128];
    size_t len = 0;
    seq_t tmp = x;
    while (tmp) {
        digits[len++] = (uint8_t)(tmp % 6);
        tmp /= 6;
    }

    unsigned int dv = 0, uv = 0;
    for (size_t i = len; i-- > 1;) {
        uint8_t curr = digits[i - 1];
        uint8_t prev = digits[i];
        if (curr > prev)
            dv += curr - prev;
        else if (curr < prev)
            uv += prev - curr;
    }
    return dv == uv;
}

static seq_t *cache   = NULL;
static size_t cache_cap = 0;
static size_t cache_size = 0;  /* last index stored */
static seq_t next_candidate = 1;

static void ensure_cache(idx_t n)
{
    if (cache_size >= (size_t)n) return;
    if (!cache) {
        cache_cap = 64;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_cap);
    }
    while (cache_size < (size_t)n) {
        if (equal_variation(next_candidate)) {
            if (cache_size + 1 >= cache_cap) {
                cache_cap *= 2;
                cache = (seq_t*)realloc(cache, sizeof(seq_t) * cache_cap);
            }
            cache[++cache_size] = next_candidate;
        }
        next_candidate++;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    ensure_cache(n);
    return cache[n];
}
