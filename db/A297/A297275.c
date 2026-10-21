/*
 * A297275  Numbers whose base-11 digits have greater up-variation than
 *          down-variation; see Comments.
 * https://oeis.org/A297275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 152, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by searching integers whose base‑11 up‑variation
   exceeds down‑variation. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* static cache of found terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t last_checked = 0;      /* last decimal integer examined */

/* ensure capacity of the cache */
static void ensure_cache_cap(void)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) exit(1);   /* allocation failure */
        cache = new_cache;
        cache_cap = new_cap;
    }
}

/* a(n): nth term of OEIS A297275 (offset 1) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for offset < 1, safe fallback */

    while (cache_len < (size_t)n) {
        /* examine next integer */
        last_checked++;
        seq_t x = (seq_t)last_checked;

        /* extract base‑11 digits, least significant first */
        int digits[64];
        size_t len = 0;
        seq_t temp = x;
        while (temp > (seq_t)0) {
            digits[len++] = (int)(temp % 11);
            temp /= 11;
        }
        if (len == 0) digits[len++] = 0;  /* handle x == 0, though never used */

        /* compute up‑variation (uv) and down‑variation (dv) */
        seq_t uv = 0, dv = 0;
        for (size_t i = 1; i < len; i++) {
            int more   = digits[i];   /* more significant digit */
            int less   = digits[i-1]; /* less significant digit */
            if (more > less) {
                dv += (seq_t)(more - less);
            } else if (less > more) {
                uv += (seq_t)(less - more);
            }
        }

        /* store term if up‑variation exceeds down‑variation */
        if (uv > dv) {
            ensure_cache_cap();
            cache[cache_len++] = x;
        }
    }
    return cache[n - 1];
}
