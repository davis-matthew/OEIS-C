/*
 * A284745  Fixed point of the morphism 0 -> 01, 1 -> 000.
 * https://oeis.org/A284745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=0; the fixed point of the morphism 0→01, 1→000 generates the sequence.
   a(n) is the nth symbol (0 or 1) of that infinite word. */

#include <stddef.h>
#include <stdlib.h>

/* static cache holding the generated word up to the largest N requested */
static unsigned char *cached = NULL;
static size_t cached_len = 0;
static size_t cached_cap = 0;

/* enlarge the cache if needed; used only when we start the cache */
static void expand_until(idx_t target)
{
    if (cached_len >= (size_t)target) return;

    /* first time, seed with the initial word "0" */
    if (cached == NULL) {
        cached_cap = 8;
        cached = (unsigned char *)malloc(cached_cap);
        cached[0] = 0;
        cached_len = 1;
    }

    /* grow until we have at least target symbols */
    while (cached_len < (size_t)target) {
        size_t new_len = 0;
        /* one expansion may add up to 3 symbols per old symbol */
        size_t temp_cap = 3 * cached_len;
        unsigned char *temp = (unsigned char *)malloc(temp_cap);
        for (size_t i = 0; i < cached_len; ++i) {
            if (cached[i] == 0) {
                temp[new_len++] = 0;
                temp[new_len++] = 1;
            } else {
                temp[new_len++] = 0;
                temp[new_len++] = 0;
                temp[new_len++] = 0;
            }
        }
        free(cached);
        cached = temp;
        cached_len = new_len;
        cached_cap = temp_cap;
    }
}

/* return the nth term of A284745 (1‑based index) */
seq_t a(idx_t n)
{
    if (n == 0) /* undefined for n<1, but caller never asks for n<1 */
        return (seq_t)0;
    expand_until(n);
    return (seq_t)cached[n - 1];
}
