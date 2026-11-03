/*
 * A352375  Sum of digits of A007618.
 * https://oeis.org/A352375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of decimal digits of A007618(n), where * A007618(1) =
 *            5 and A007618(n+1) - A007618(n) = a(n)
 * Range:     u8 n <= 28, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-03)
 * Helpers:   digit_sum, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"
#include "../utils/mem.h"

static seq_t *s_values = NULL;   /* stores A007618(n), 1‑based index -> array[0] = n=1 */
static size_t s_len  = 0;        /* current number of stored terms (len >= 1 if initialized) */
static size_t s_cap  = 0;        /* allocated capacity for s_values */

static void extend_to(idx_t n)
{
    /* Ensure internal buffer contains terms 1..n of A007618. */
    if (n == 0) return; /* undefined, but guard anyway */

    /* First‑time initialization, set s(1)=5. */
    if (s_len == 0) {
        s_cap = 4;
        s_values = mem_new(seq_t, s_cap);
        if (!s_values) abort();               /* allocation failure */
        s_values[0] = (seq_t)5;                /* A007618(1) */
        s_len = 1;
    }

    while (s_len < (size_t)n) {
        seq_t last = s_values[s_len - 1];
        seq_t inc  = digit_sum(last);        /* a(s_len) = digit_sum(last) */
        seq_t next = last + inc;             /* A007618(s_len+1) */

        if (s_len == s_cap) {                /* grow buffer if needed */
            s_cap *= 2;
            s_values = mem_resize(seq_t, s_values, s_cap);
            if (!s_values) abort();
        }
        s_values[s_len] = next;
        ++s_len;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* out of bounds; unused by specification */
    extend_to(n);                  /* ensure A007618(n) is available */
    seq_t val = s_values[(size_t)n - 1];
    return digit_sum(val);         /* a(n) = sum of digits of A007618(n) */
}
