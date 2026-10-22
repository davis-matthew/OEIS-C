/*
 * A340238  Odd composite integers m such that A054413(3*m-J(m,53)) == 7 (mod
 *          m), where J(m,53) is the Jacobi symbol.
 * https://oeis.org/A340238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   is_prime, jacobi_symbol, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  a(n) : nth odd composite integer m with
 *         A054413(3*m - J(m,53)) ≡ 7 (mod m)
 *
 *  Implementation: generate odd composites in increasing order and
 *  test the Lucas–pseudoprime condition.  The results are cached
 *  so that calls with arbitrary n order are handled efficiently.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include "../utils/jacobi_symbol.h"     /* int jacobi_symbol(uint64_t a, uint64_t n); */
#include "../utils/is_prime.h"          /* bool is_prime(uint64_t n); */
#include "../utils/seq_realloc.h"       /* seq_t *seq_realloc(seq_t *old, size_t old_cap, size_t new_cap); */

/* ---------- Helper for 128‑bit modular multiplication ---------- */
static inline uint64_t mul_mod_u64(uint64_t a, uint64_t b, uint64_t mod)
{
    return ((unsigned __int128)a * b) % mod;
}

/* ---------- Lucas sequence U(n) modulo mod (mod fits in 64 bits) ---------- */
static seq_t lucas_U_mod(idx_t n, seq_t mod)
{
    if (n == 0) return 0;
    if (n == 1) return 1 % mod;

    /* Use 64‑bit arithmetic because mod < 2^64. */
    uint64_t M = (uint64_t)mod;
    uint64_t a = 1;   /* identity matrix element a */
    uint64_t b = 0;
    uint64_t c = 0;
    uint64_t d = 1;

    uint64_t ra = 1, rb = 0, rc = 0, rd = 1;      /* result matrix R (identity) */
    uint64_t ba = 7 % M, bb = 1 % M, bc = 1 % M, bd = 0 % M; /* base matrix */

    uint64_t e = n - 1;
    while (e)
    {
        if (e & 1)
        {
            uint64_t na = (mul_mod_u64(ra, ba, M) + mul_mod_u64(rb, bc, M)) % M;
            uint64_t nb = (mul_mod_u64(ra, bb, M) + mul_mod_u64(rb, bd, M)) % M;
            uint64_t nc = (mul_mod_u64(rc, ba, M) + mul_mod_u64(rd, bc, M)) % M;
            uint64_t nd = (mul_mod_u64(rc, bb, M) + mul_mod_u64(rd, bd, M)) % M;
            ra = na; rb = nb; rc = nc; rd = nd;
        }
        uint64_t na = (mul_mod_u64(ba, ba, M) + mul_mod_u64(bb, bc, M)) % M;
        uint64_t nb = (mul_mod_u64(ba, bb, M) + mul_mod_u64(bb, bd, M)) % M;
        uint64_t nc = (mul_mod_u64(bc, ba, M) + mul_mod_u64(bd, bc, M)) % M;
        uint64_t nd = (mul_mod_u64(bc, bb, M) + mul_mod_u64(bd, bd, M)) % M;
        ba = na; bb = nb; bc = nc; bd = nd;
        e >>= 1;
    }

    return (seq_t)ra;   /* U(n)  */
}

/* ---------- Cache for found terms ---------- */
static seq_t *cache   = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t next_candidate = 9;   /* first odd composite to test */

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n=0 – return 0 for safety */

    /* Grow cache until it contains at least n terms. */
    while (cache_len < (size_t)n)
    {
        /* Find the next odd composite that satisfies the Lucas condition. */
        for (;;)
        {
            uint64_t m = next_candidate;
            next_candidate += 2;          /* only odd numbers */

            if (m < 9) continue;          /* skip 1,3,5,7 */
            if (is_prime(m)) continue;    /* skip primes */

            int j = jacobi_symbol(m, 53);   /* Jacobi symbol J(m,53) */
            idx_t exp = 3ULL * m - (idx_t)j;

            seq_t mod = (seq_t)m;
            seq_t val = lucas_U_mod(exp, mod);

            if (val == (seq_t)7)          /* 7 < m for all candidates */
            {
                /* Ensure storage space. */
                if (cache_len == cache_cap)
                {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                    seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                    if (!tmp) abort();    /* allocation failure */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)m;
                break;                    /* next term ready */
            }
        }
    }

    return cache[(size_t)n - 1];
}
