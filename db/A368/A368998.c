/*
 * A368998  Numbers k such that A003415(k) and A276085(k) are both even, where
 *          A003415 is the arithmetic derivative and A276085 is the primorial
 *          base log-function; Numbers k for which A342001(k) is even.
 * https://oeis.org/A368998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 9616, u32 n <= 9616, u64 n <= 9616,
 *            u128 n <= 9616, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A368998: numbers k such that the arithmetic derivative A003415(k)
   and the primorial log–function A276085(k) are both even.
   Characterization used:
   - Let e₂ be the exponent of 2 in k.
   - If e₂ is odd → k is excluded.
   - If e₂ > 0 (and even) → k is automatically included.
   - If e₂ == 0 (k odd) let s be the sum of the exponents of all odd
     prime factors of k; k is included iff s is even.  (k = 1 gives s = 0.)  */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Cache for computed sequence values. */
static seq_t *cache = NULL;     /* array of terms */
static size_t cap   = 0;        /* allocated capacity */
static size_t len   = 0;        /* number of terms stored */
static idx_t last_checked = 0;  /* largest k examined */

/* Ensure the cache can hold at least 'needed' terms. */
static void ensure_capacity(size_t needed)
{
    if (cap < needed) {
        size_t new_cap = cap ? cap << 1 : 16;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();              /* failure is unrecoverable */
        cache = tmp;
        cap   = new_cap;
    }
}

/* Return true if k satisfies the parity conditions. */
static bool candidate_ok(idx_t k)
{
    /* Count the exponent of 2 in k. */
    idx_t e2 = 0;
    idx_t t = k;
    while (((t & 1u) == 0)) {
        t >>= 1;
        ++e2;
    }
    if (e2 & 1u)          /* e₂ odd → reject */
        return false;
    if (e2 > 0)            /* e₂ even and positive → accept */
        return true;

    /* e₂ == 0: k is odd.  Sum exponents of odd prime factors. */
    idx_t sum = 0;
    idx_t p = 3;
    while (p <= t / p) {
        while (t % p == 0) {
            ++sum;
            t /= p;
        }
        p += 2;
    }
    if (t > 1) /* remaining prime factor */
        ++sum;
    return ((sum & 1u) == 0);
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined, but safe fallback */

    /* Generate terms until we have at least n of them. */
    while (len < n) {
        ++last_checked;
        if (candidate_ok(last_checked)) {
            ensure_capacity(len + 1);
            cache[len] = (seq_t)last_checked;
            ++len;
        }
    }
    return cache[n - 1];
}
