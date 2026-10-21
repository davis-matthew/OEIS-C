/*
 * A246158  Odious reducible polynomials over GF(2), coded in binary.
 *          (Polynomials with an odd number of nonzero terms that are
 *          reducible over GF(2)).
 * https://oeis.org/A246158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 521, u32 n <= 9703, u64 n <= 9703,
 *            u128 n <= 9703, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A246158: the n‑th integer whose binary encoding represents
   a polynomial over GF(2) with an odd number of non‑zero terms
   that is reducible over GF(2).  The function searches integers in
   ascending order and tests each one for the required property. */

/* Required utilities */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/bitlen_seq_t.h"

/* ---------- popcount (Hamming weight) for seq_t ---------- */
static unsigned popcount_seq(seq_t x)
{
    unsigned c = 0;
    while (x) {
        x &= x - (seq_t)1;   /* clear lowest set bit */
        ++c;
    }
    return c;
}

/* ---------- polynomial modulo (shifted XOR) ---------- */
static seq_t poly_mod(seq_t a, seq_t mod, unsigned deg_mod)
{
    seq_t r = a;
    while (r && ((unsigned)(bitlen_seq_t(r) - 1) >= deg_mod)) {
        unsigned shift = (unsigned)(bitlen_seq_t(r) - 1) - deg_mod;
        r ^= mod << shift;
    }
    return r;
}

/* ---------- gcd of polynomials over GF(2) ---------- */
static seq_t poly_gcd(seq_t a, seq_t b)
{
    while (b) {
        unsigned deg_b = (unsigned)(bitlen_seq_t(b) - 1);
        seq_t r = poly_mod(a, b, deg_b);
        a = b;
        b = r;
    }
    return a;
}

/* ---------- square a polynomial modulo 'mod' ---------- */
static seq_t poly_square_mod(seq_t g, seq_t mod, unsigned deg_mod)
{
    seq_t sq = 0;
    seq_t t = g;
    unsigned i = 0;
    while (t) {
        if (t & (seq_t)1) {
            sq ^= (seq_t)1 << (2 * i);
        }
        t >>= 1;
        ++i;
    }
    return poly_mod(sq, mod, deg_mod);
}

/* ---------- test whether a polynomial is irreducible over GF(2) ---------- */
static bool is_irreducible(seq_t p)
{
    if (p == (seq_t)0) return false;                 /* zero polynomial */

    unsigned deg = (unsigned)(bitlen_seq_t(p) - 1);
    if (deg <= 1) return true;                      /* degree 0 or 1 */

    /* Check for roots at 0 and 1 ------------------------------------ */
    if ((p & (seq_t)1) == (seq_t)0) return false;   /* root at 0 */
    if (popcount_seq(p) % 2 == 0) return false;      /* root at 1 */

    /* Berlekamp/standard irreducibility test ------------------------ */
    seq_t g = (seq_t)2;            /* x */
    unsigned half = deg / 2;
    for (unsigned i = 1; i <= half; ++i) {
        g = poly_square_mod(g, p, deg);
        seq_t h = g ^ (seq_t)2;  /* g - x */
        seq_t d = poly_gcd(p, h);
        if (d != (seq_t)1) return false;   /* common factor found */
    }
    return true;
}

/* ---------- test for "odious reducible" over GF(2) ---------- */
static bool is_odious_reducible(seq_t n)
{
    if (n == (seq_t)0) return false;

    /* odd number of 1-bits (odious) */
    if (popcount_seq(n) % 2 == 0) return false;

    unsigned deg = (unsigned)(bitlen_seq_t(n) - 1);
    if (deg <= 1) return false;           /* cannot be reducible */

    return !is_irreducible(n);            /* reducible over GF(2) */
}

/* ---------- main sequence function ------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                 /* sequence starts at 1 */

    static seq_t *cache = NULL;
    static size_t cache_cnt = 0;
    static size_t cache_cap = 0;
    static seq_t last_candidate = 1;             /* next integer to test */

    /* initialise cache on first call */
    if (!cache) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;             /* allocation failure */
    }

    /* generate terms until we have at least n cached */
    while (cache_cnt < (size_t)n) {
        seq_t cand = last_candidate++;
        if (is_odious_reducible(cand)) {
            /* ensure capacity */
            if (cache_cnt >= cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {
                    free(cache);
                    cache = NULL;
                    cache_cap = 0;
                    return (seq_t)0;
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_cnt++] = cand;
        }
    }
    return cache[n - 1];
}
