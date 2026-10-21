/*
 * A297286  Numbers whose base-15 digits have equal down-variation and
 *          up-variation; see Comments.
 * https://oeis.org/A297286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = nth integer such that base‑15 down‑variation =
 *            up‑variation * (computed by brute‑force search)
 * Range:     u8 n <= 30, u16 n <= 4382, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#if defined(__cplusplus)
#include <new>
#endif

static seq_t *seq_values = NULL;   /* array of found terms */
static size_t   seq_len   = 0;      /* number of terms stored      */
static size_t   seq_cap   = 0;      /* capacity of the array       */
static seq_t   last_scan  = 0;      /* last integer examined       */

/* Ensure capacity for at least 'need' elements. */
static void ensure_capacity(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 64;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(seq_values, new_cap * sizeof(seq_t));
    seq_values = tmp;
    seq_cap    = new_cap;
}

/* Check if n has equal down‑variation and up‑variation in base 15. */
static int balanced_x15(seq_t n)
{
    if (n == 0) return 0;
    seq_t tmp = n;
    seq_t last_digit = tmp % 15;
    tmp /= 15;
    seq_t up   = 0;
    seq_t down = 0;
    while (tmp > 0)
    {
        seq_t digit = tmp % 15;
        if (digit > last_digit)
            down += digit - last_digit;
        else if (digit < last_digit)
            up   += last_digit - digit;
        last_digit = digit;
        tmp /= 15;
    }
    return up == down;
}

/* Return the n‑th term of the sequence (a(1) is the first term). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    while (seq_len < n)
    {
        ++last_scan;
        if (balanced_x15(last_scan))
        {
            ensure_capacity(seq_len + 1);
#ifdef __cplusplus
            new ((void*)&seq_values[seq_len]) seq_t(last_scan);
#else
            seq_values[seq_len] = last_scan;
#endif
            seq_len++;
        }
    }
    return seq_values[n - 1];
}
