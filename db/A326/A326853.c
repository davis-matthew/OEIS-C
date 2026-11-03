/*
 * A326853  BII-numbers of set-systems where every two covered vertices appear
 *          together in some edge (cointersecting).
 * https://oeis.org/A326853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 66 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: the n‑th BII‑number whose set‑system is cointersecting.
   The sequence is defined by searching over non‑negative integers and
   keeping those whose BII‑derived set‑system satisfies the
   cointersecting property.  The function is memoised so that calls in
   arbitrary order are fast. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Test whether the set‑system of the BII‑number n is cointersecting.
   This follows directly from the definition: for every vertex u,
   every pair of vertices that co‑appear in any edge together with u
   must belong to the union of all edges incident to u.  The algorithm
   builds the vertices (bits 0..63) and the edges described by the
   binary indices of the positions of the 1‑bits in n.  It then verifies
   the cointersecting condition. */
static bool is_cointersecting(seq_t n)
{
    const seq_t one = 1;
    seq_t       m = n;
    uint64_t    e_masks[64];
    unsigned    e_count = 0;
    uint64_t    V = 0;                 /* union of all vertices in edges   */

    /* enumerate all 1‑bits of n; the position is 1‑based */
    idx_t pos = 1;
    while (m != 0)
    {
        if (m & one)                /* bit at position pos is set      */
        {
            /* compute mask of binary indices of pos (edge mask) */
            uint64_t e_mask = 0;
            uint64_t v     = pos;
            unsigned bit_v = 1;      /* values for vertices start at 1   */
            while (v != 0)
            {
                if (v & 1ULL)
                    e_mask |= 1ULL << (bit_v - 1);
                v >>= 1;
                bit_v++;
            }
            e_masks[e_count++] = e_mask;
            V |= e_mask;              /* add vertices of this edge to V   */
        }
        m >>= 1;
        pos++;
    }

    /* For each vertex u in V compute the union of all edges that
       contain u.  If for any u this union differs from V, the system
       is not cointersecting. */
    uint64_t U[64] = {0};

    for (unsigned i = 0; i < e_count; ++i)
    {
        uint64_t e = e_masks[i];
        uint64_t tmp = e;
        while (tmp)
        {
            unsigned lsb = 0;
            while (((tmp >> lsb) & 1ULL) == 0)   /* find lowest set bit */
                lsb++;
            U[lsb] |= e;
            tmp &= tmp - 1;                       /* clear lowest bit  */
        }
    }

    uint64_t tmpV = V;
    while (tmpV)
    {
        unsigned u = 0;
        while (((tmpV >> u) & 1ULL) == 0)
            u++;
        if (U[u] != V)
            return false;
        tmpV &= tmpV - 1;
    }
    return true;
}

/* Cache of already found terms (memoisation).  We store the terms
   directly in an array of seq_t, so the cache works for both
   integral and arbitrary‑precision builds. */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static size_t cache_len  = 0;
static seq_t next_candidate = 0;

/* Return the n‑th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    /* Ensure we have computed at least n terms. */
    while (cache_len < (size_t)n)
    {
        if (is_cointersecting(next_candidate))
        {
            /* Grow cache if necessary. */
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *new_arr = mem_resize(seq_t, cache, new_cap);
                if (new_arr == NULL) abort();      /* out of memory */
                cache      = new_arr;
                cache_cap  = new_cap;
            }
            cache[cache_len++] = next_candidate;
        }
        ++next_candidate;
    }
    /* Return the stored value.  Casting is trivial for integral
       seq_t types and is a no‑op for arbitrary‑precision types. */
    return cache[(size_t)n - 1];
}
