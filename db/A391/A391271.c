/*
 * A391271  Negated discriminants of imaginary quadratic fields F such that 16
 *          is an 8th power in F_2, where F_2 is the completion of F with
 *          respect to a place above 2.
 * https://oeis.org/A391271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 3316, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A391271
 *
 * A(n) is the positive integer -d where d<0 is a fundamental discriminant
 * congruent to 8,28,56,60 modulo 64.
 *
 * Equivalently, let m = a(n)/4.  Then m is square‑free and m ≡ 1,2,9,14 (mod 16).
 * The sequence contains 4*m for all such m in increasing order.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* auxiliary: test whether m is square‑free (m ≤ ~2^32 for n ≤ 10 000) */
static bool squarefree(idx_t m)
{
    if (m <= 1)
        return true;

    /* factor 2 separately */
    if ((m & 3) == 0)          /* divisible by 4 → not square‑free */
        return false;
    while ((m & 1) == 0)
        m >>= 1;              /* remove all factors of 2 */

    /* test odd primes up to sqrt(m) */
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            m /= p;            /* remove one factor */
            if (m % p == 0)    /* p^2 divides original m */
                return false;
            while (m % p == 0) /* remove remaining powers of p */
                m /= p;
        }
    }
    return true;
}

/* run‑time generated cache of terms */
static seq_t *cache     = NULL;
static size_t cache_len = 0, cache_cap = 0;
static idx_t last_m     = 0;          /* last m tested */

/* append one term to the cache */
static void append_term(seq_t val)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 64 : cache_cap << 1;
        seq_t *new_buf = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_buf) abort();        /* out of memory */
        cache   = new_buf;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined, return 0 for safety */

    /* grow the cache until it holds at least n terms */
    while (cache_len < n) {
        idx_t m = ++last_m;        /* candidate m */

        /* require m ≡ 1,2,9,14 (mod 16) */
        idx_t r = m & 15;           /* modulo 16 fast */
        if (r != 1 && r != 2 && r != 9 && r != 14)
            continue;

        if (!squarefree(m))
            continue;

        /* legal term: a = 4 * m */
        seq_t val = (seq_t)4 * (seq_t)m;
        append_term(val);
    }
    return cache[n - 1];
}
