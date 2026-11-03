/*
 * A297261  Numbers whose base-7 digits have greater down-variation than
 *          up-variation; see Comments.
 * https://oeis.org/A297261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n‑th integer whose base‑7 down‑variation exceeds its
 *            up‑variation. The implementation enumerates integers, checks
 *            this property, and caches results for repeated queries
 * Range:     u8 n <= 100, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 9187 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Return true iff the base‑7 down‑variation of x is greater than its up‑variation. */
static bool isgood(seq_t x)
{
    if (x == (seq_t)0) return false;

    /* Extract base‑7 digits (least significant first). */
    unsigned char da[64];
    size_t len = 0;
    seq_t t = x;
    while (t > (seq_t)0) {
        seq_t r = t % (seq_t)7;
        da[len++] = (unsigned char)((int)(r));
        t = t / (seq_t)7;
    }
    if (len == 0) da[len++] = 0;   /* x==0 case, never happens here */

    seq_t uv = (seq_t)0, dv = (seq_t)0;

    for (size_t i = len-1; i > 0; --i) {
        unsigned int hi = da[i];
        unsigned int lo = da[i-1];
        if (hi > lo)
            dv += (seq_t)(hi - lo);
        else if (lo > hi)
            uv += (seq_t)(lo - hi);
    }
    return dv > uv;
}

/* Cache of computed terms. */
static seq_t *terms   = NULL;
static size_t terms_cap = 0;
static size_t terms_len = 0;
static seq_t next_candidate = (seq_t)0;

/* Ensure the cache has room for at least 'need' elements. */
static void ensure_cache(size_t need)
{
    if (need <= terms_cap)
        return;
    size_t newcap = terms_cap ? terms_cap*2 : 16;
    while (newcap < need)
        newcap *= 2;
    terms = mem_resize(seq_t, terms, newcap);
    terms_cap = newcap;
}

/* Return the n-th term of A297261 (offset 1). */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;       /* undefined for the offset, return 0 */

    while (terms_len < (size_t)n) {
        next_candidate = next_candidate + (seq_t)1;
        if (isgood(next_candidate)) {
            ensure_cache(terms_len + 1);
            terms[terms_len++] = next_candidate;
        }
    }
    return terms[(size_t)(n-1)];
}
