/*
 * A396012  Number of 3's in the partitions of n into exactly 5 parts.
 * https://oeis.org/A396012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) - 2*a(n-5) + a(n-8) + a(n-9) - a(n-10)
 *            for n > 25. For n ≤ 25 the values are obtained by brute‑force
 *            enumeration of all partitions of n into exactly five (unordered)
 *            parts, counting how many 3’s occur in each
 * Range:     u8 n <= 32, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"   /* seq_alloc_new_array(size_t) */
#include "../utils/seq_free_array.h"        /* seq_free_array(seq_t*) */

/* Brute‑force counting for the first 26 values (n ≤ 25). */
static seq_t compute_base(idx_t n)
{
    if (n < 5) return 0;
    seq_t total = 0;
    for (idx_t a = 1; a <= n - 4; ++a) {
        for (idx_t b = a; b <= n - 3 - a; ++b) {
            for (idx_t c = b; c <= n - 2 - a - b; ++c) {
                for (idx_t d = c; d <= n - 1 - a - b - c; ++d) {
                    idx_t e = n - a - b - c - d;
                    if (e < d) continue;          /* ensure non‑decreasing order */
                    seq_t cnt = 0;
                    if (a == 3) ++cnt;
                    if (b == 3) ++cnt;
                    if (c == 3) ++cnt;
                    if (d == 3) ++cnt;
                    if (e == 3) ++cnt;
                    total += cnt;
                }
            }
        }
    }
    return total;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Static cache for previously computed values */
    static size_t  cap  = 0;
    static seq_t *buf  = NULL;
    static int    ready = 0;   /* 0 → not initialised, 1 → ready */

    /* Expand cache to hold indices up to n (if necessary) */
    if (!ready || (size_t)n >= cap) {
        size_t newcap = (size_t)n + 1;
        if (newcap < 26) newcap = 26;            /* pre‑compute up to 25 */
        seq_t *newbuf = seq_alloc_new_array(newcap);
        if (!newbuf) return 0;                  /* allocation failure */

        /* Copy old values if any */
        for (size_t i = 0; i < cap; ++i) newbuf[i] = buf[i];

        /* Initialise missing entries */
        for (size_t i = cap; i < newcap; ++i) {
            if (i <= 25) {
                newbuf[i] = compute_base((idx_t)i);
            } else {
                seq_t s1 = newbuf[i-1] + newbuf[i-2] + newbuf[i-8] + newbuf[i-9];
                seq_t res = s1 - newbuf[i-10];
                seq_t two = newbuf[i-5];
                two += two;                       /* multiply by 2 */
                res -= two;
                newbuf[i] = res;
            }
        }

        /* Free old buffer if it existed */
        if (buf) seq_free_array(buf);

        buf  = newbuf;
        cap  = newcap;
        ready = 1;
    }

    return buf[(size_t)n];
}
