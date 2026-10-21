/*
 * A241024  S_11 sequence in partition of integers > 1 described in A240521.
 * https://oeis.org/A241024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   floor_sqrt, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = A050376(n) * A050376(n+2) * A050376(n+3)
 *
 * A050376 is defined as follows:
 *   a(1) = 2; thereafter the next term is the smallest integer > the
 *   previous term that is either prime or a perfect square whose square-
 *   root has appeared earlier in the sequence.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include "../utils/floor_sqrt.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* ------------------------------------------------------------ */
/* small prime test for 64‑bit integers (used for values that
 * comfortably fit in uint64_t).  The sequence values for the
 * required range are much smaller than 64 bit, so this is
 * sufficient. */
static inline bool is_prime_uint64(uint64_t n)
{
    if (n < 2)                return false;
    if (n % 2 == 0)           return n == 2;
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0)       return false;
    }
    return true;
}
/* ------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1 */

    /* ---------- A050376 cache -------------------------------- */
    static seq_t *t        = NULL;   /* 1‑based array, t[0] dummy */
    static size_t t_len    = 0;      /* number of stored elements (incl. dummy) */
    static size_t t_cap    = 0;      /* capacity of t */

    /* ---------- presence bit‑set for numbers that have appeared */
    static unsigned char *present  = NULL;   /* present[v]==1 if v∈t */
    static size_t        p_cap     = 0;      /* allocated capacity */

    /* ---------- one‑time initialisation ---------------------- */
    if (t == NULL) {
        /* initial allocation */
        t_cap = 16;
        t     = seq_alloc(t_cap);
        t[0]  = (seq_t)0;              /* dummy index 0 */

        /* first four terms (known by definition) */
        t[1] = (seq_t)2;
        t[2] = (seq_t)3;
        t[3] = (seq_t)4;
        t[4] = (seq_t)5;
        t_len = 5;                     /* indices 0 … 4 are valid */

        /* presence array initialisation */
        p_cap   = 16;
        present = (unsigned char *)malloc(p_cap);
        for (size_t i = 0; i < p_cap; ++i) present[i] = 0;
        present[2] = present[3] = present[4] = present[5] = 1;
    }

    /* ---------- grow cache until we have at least n+3 terms ----- */
    while (t_len <= (size_t)n + 3) {
        seq_t last  = t[t_len - 1];
        seq_t cand  = last + (seq_t)1;

        /* search for next term */
        for (;;) {
            /* prime test */
            bool prime_ok = is_prime_uint64( (uint64_t)cand );

            /* square test */
            bool square_ok = false;
            seq_t sq = floor_sqrt(cand);
            if (sq * sq == cand) {
                uint64_t r = (uint64_t)sq;
                if (r < p_cap) square_ok = present[r] != 0;
            }

            if (prime_ok || square_ok) break;  /* candidate accepted */
            ++cand;
        }

        /* ensure enough capacity for t */
        if (t_len >= t_cap) {
            size_t new_cap = t_cap * 2;
            t = seq_realloc(t, t_cap, new_cap);
            t_cap = new_cap;
        }
        t[t_len] = cand;
        ++t_len;

        /* ensure present array can index the new value */
        uint64_t val = (uint64_t)cand;
        if (val >= p_cap) {
            size_t new_cap = p_cap;
            while (val >= new_cap) new_cap *= 2;
            present = (unsigned char *)realloc(present, new_cap);
            for (size_t i = p_cap; i < new_cap; ++i) present[i] = 0;
            p_cap = new_cap;
        }
        present[val] = 1;
    }

    /* ---------- compute a(n) -------------------------------- */
    seq_t p = t[n]      * t[n + 2];
    p       = p * t[n + 3];
    return p;
}
