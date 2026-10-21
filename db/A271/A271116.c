/*
 * A271116  Integers n such that round(3^n/12) is divisible by n.
 * https://oeis.org/A271116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 131, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A271116: a(n) is the n‑th integer k such that round(3^k/12) is
   divisible by k (equivalently, 3^(k-1) ≡ ±1 (mod 4k)). */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"
#include "../utils/seq_realloc.h"

/* Cache for the sequence values */
static seq_t *cache     = NULL;
static size_t cap       = 0;
static size_t len       = 0;          /* number of terms stored */
static idx_t  next_cand = 1;          /* next candidate k to test */

/* Test whether a given k satisfies the A271116 condition. */
static bool
passes(idx_t k)
{
    if (k == 0) return false;                 /* 0 is not used */

    idx_t    exp = k - 1;                     /* exponent for 3^k */
    /* modulus m = 4*k (type seq_t; avoids C int overflow) */
    seq_t    m   = ((seq_t)4) * ((seq_t)k);

    /* compute 3^(k-1) mod m */
    seq_t    pow = mod_pow((seq_t)3, (uint64_t)exp, m);

    /* odd k requires 3^(k-1) ≡ 1 (mod m); even k requires ≡ -1 */
    if (k & 1ULL) {                           /* k is odd */
        return pow == (seq_t)1;
    } else {                                  /* k is even */
        return pow == (m - (seq_t)1);        /* m-1 == -1 mod m */
    }
}

/* Append a new term to the cache, resizing if necessary. */
static void
append_term(seq_t val)
{
    if (len == cap) {
        size_t newcap = (cap == 0) ? 16 : cap * 2;
        cache = seq_realloc(cache, cap, newcap);
        cap   = newcap;
    }
    cache[len++] = val;
}

/* Return the n-th term of A271116 (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;               /* undefined, return 0 */

    while (len < n) {
        idx_t k = next_cand++;
        if (passes(k))
            append_term((seq_t)k);
    }
    return cache[n - 1];
}
