/*
 * A385655  Numbers k >= 0 such that k = digsum(k) + digsum(k+1) + ... +
 *          digsum(k+r) for some r >= 0 where digsum(i) is the digital sum of
 *          i (A007953).
 * https://oeis.org/A385655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the nth non‑negative integer k such that k =
 *            digit_sum(k) + digit_sum(k+1) + … + digit_sum(k+r) for some r ≥
 *            0, where digit_sum(i) is the sum of decimal digits of i
 * Range:     u8 n <= 59, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *term_cache = NULL;   /* cached terms     */
static size_t term_len   = 0;      /* number of terms  */
static size_t term_cap   = 0;      /* capacity of cache */

/* Ensure the cache can hold at least NEED elements. */
static void
ensure_capacity(size_t need)
{
    if (need <= term_cap) return;
    size_t newcap = term_cap ? term_cap * 2 : 8;
    while (newcap < need) newcap <<= 1;
    seq_t *tmp = (seq_t *)realloc(term_cache, newcap * sizeof(seq_t));
    if (!tmp) abort();             /* allocation failure */
    term_cache = tmp;
    term_cap   = newcap;
}

/* Compute the decimal digit sum of a 64‑bit integer. */
static unsigned digit_sum_u64(idx_t v)
{
    unsigned sum = 0;
    while (v) {
        sum += (unsigned)(v % 10u);
        v   /= 10u;
    }
    return sum;
}

/* Find and return the nth term of A385655.  The offset is 1:
   a(1) == 0.  The function is memoised, so repeated calls are fast. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined offset – harmless */

    /* Ensure we have computed enough terms. */
    if (term_len < (size_t)n) {
        /* Start searching from the next integer after the last found term. */
        idx_t k = (term_len ? (idx_t)term_cache[term_len - 1] + 1ULL : 0ULL);

        while (term_len < (size_t)n) {
            idx_t sum = 0;
            bool ok   = false;

            /* Try increasing r until the sum exceeds k or matches it. */
            for (idx_t r = 0; ; ++r) {
                idx_t val = k + r;
                sum += digit_sum_u64(val);
                if (sum == k) { ok = true; break; }
                if (sum >  k) break;
            }

            if (ok) {
                ensure_capacity(term_len + 1);
                term_cache[term_len++] = (seq_t)k;
            }
            ++k;
        }
    }
    return term_cache[n - 1];
}
