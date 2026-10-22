/*
 * A369664  Numbers k of the form 4m+1, whose arithmetic derivative k' is of
 *          the form 4u+2, and k' has an odd number of prime factors.
 * https://oeis.org/A369664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 3040, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation by brute‑force search: a(n) = 4m+1 where k′ ≡ 2 (mod 4) and
 * k′ has an odd number of prime factors (counted with multiplicity). */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* trial division factorisation of a 64‑bit integer */
static void
factor_uint64(uint64_t n, uint64_t *pr, int *ex, size_t *cnt)
{
    size_t i = 0;
    if (n < 2) { *cnt = 0; return; }

    if (n % 2 == 0) {
        pr[i] = 2;
        ex[i] = 0;
        while (n % 2 == 0) { ++ex[i]; n /= 2; }
        ++i;
    }

    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            pr[i] = p;
            ex[i] = 0;
            while (n % p == 0) { ++ex[i]; n /= p; }
            ++i;
        }
    }

    if (n > 1) {
        pr[i] = n; ex[i] = 1; ++i;
    }
    *cnt = i;
}

/* count the total number (with multiplicity) of prime factors of a 64‑bit integer */
static int
prime_factor_count(uint64_t n)
{
    uint64_t pr[64];
    int ex[64];
    size_t cnt;
    factor_uint64(n, pr, ex, &cnt);
    int sum = 0;
    for (size_t i = 0; i < cnt; ++i) sum += ex[i];
    return sum;
}

/* ------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t   len   = 0;
    static size_t   cap   = 0;
    static idx_t    next_m = 0;   /* start candidate m */

    /* grow cache until we have at least n terms */
    while (len < (size_t)n) {
        /* next candidate of the form 4m+1 */
        uint64_t m = next_m++;
        uint64_t k = 4 * m + 1;
        if (k <= 1) continue;      /* skip the trivial k = 1 */

        /* factorise k */
        uint64_t pr[64];
        int     ex[64];
        size_t  cnt;
        factor_uint64(k, pr, ex, &cnt);

        /* compute arithmetic derivative k' = Σ a_i * (k / p_i) */
        uint64_t der = 0;
        for (size_t i = 0; i < cnt; ++i) {
            der += (uint64_t)ex[i] * (k / pr[i]);
        }

        /* check that k' ≡ 2 (mod 4) */
        if (der % 4 != 2) continue;

        /* prime factor count of k' */
        int omega = prime_factor_count(der);
        if ((omega & 1) == 0) continue;   /* need odd count */

        /* store valid term */
        if (len + 1 > cap) {
            size_t new_cap = (cap == 0) ? 16 : cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();         /* allocation failed */
            cache = tmp;
            cap   = new_cap;
        }
        cache[len++] = (seq_t)k;
    }

    return cache[n - 1];  /* 1‑based indexing */
}
