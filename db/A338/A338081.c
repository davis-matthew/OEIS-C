/*
 * A338081  Odd composite integers such that A054413(m)^2 == 1 (mod m).
 * https://oeis.org/A338081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-01)
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A338081:
   Odd composite integers m such that U(m)^2 ≡ 1 (mod m)
   where U is the Lucas sequence with parameters a = 7, b = -1:
       U(0) = 0, U(1) = 1,
       U(k+2) = 7·U(k+1) + U(k).
   The function searches odd composite numbers in increasing order
   and memoises them so that any call to a(n) is O(1) after the
   required terms have been generated.  */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_seq.h"

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* modulo arithmetic for the Lucas sequence elements.
   Matrix exponentiation is used to compute U(n) mod n efficiently.
   The recurrence U(k+2) = 7·U(k+1) + U(k) can be represented by
   the matrix M = [[0,1],[1,7]] such that [U(k+1),U(k)]^T = M·[U(k),U(k-1)]^T. */
static inline void mul_mat(uint64_t a00, uint64_t a01, uint64_t a10, uint64_t a11,
                           uint64_t b00, uint64_t b01, uint64_t b10, uint64_t b11,
                           uint64_t mod,
                           uint64_t *r00, uint64_t *r01, uint64_t *r10, uint64_t *r11)
{
    uint64_t t00 = ((unsigned __int128)a00 * b00 + (unsigned __int128)a01 * b10) % mod;
    uint64_t t01 = ((unsigned __int128)a00 * b01 + (unsigned __int128)a01 * b11) % mod;
    uint64_t t10 = ((unsigned __int128)a10 * b00 + (unsigned __int128)a11 * b10) % mod;
    uint64_t t11 = ((unsigned __int128)a10 * b01 + (unsigned __int128)a11 * b11) % mod;
    *r00 = t00; *r01 = t01; *r10 = t10; *r11 = t11;
}

static uint64_t U_mod_func(uint64_t n, uint64_t mod)
{
    if (n == 0) return 0ULL % mod;
    if (n == 1) return 1ULL % mod;

    uint64_t e = n - 1;               /* exponent for M^(n-1) */
    uint64_t r00 = 1, r01 = 0, r10 = 0, r11 = 1; /* identity */
    uint64_t b00 = 0, b01 = 1, b10 = 1, b11 = 7 % mod; /* M */

    while (e) {
        if (e & 1U) {
            uint64_t t00, t01, t10, t11;
            mul_mat(r00, r01, r10, r11, b00, b01, b10, b11, mod,
                    &t00, &t01, &t10, &t11);
            r00 = t00; r01 = t01; r10 = t10; r11 = t11;
        }
        /* square base matrix */
        uint64_t t00, t01, t10, t11;
        mul_mat(b00, b01, b10, b11, b00, b01, b10, b11, mod,
                &t00, &t01, &t10, &t11);
        b00 = t00; b01 = t01; b10 = t10; b11 = t11;
        e >>= 1;
    }
    return r11 % mod;   /* U(n) is the lower-right entry */
}

/* ---------- memoisation infrastructure ---------- */
static seq_t *cache = NULL;
static size_t  cache_len = 0;
static size_t  cache_cap = 0;
static uint64_t next_candidate = 3;   /* odd numbers start at 3 */

static void cache_append(uint64_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
        seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
        if (!tmp) abort();                 /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = (seq_t)val;        /* cast to seq_t */
}

/* ---------- public interface ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* offset 1; not used */

    /* generate terms until we have at least n of them */
    while (cache_len < n) {
        uint64_t m = next_candidate;
        /* odd composite check */
        if ( (m & 1ULL) && !is_prime_seq(m) ) {
            uint64_t u = U_mod_func(m, m);
            seq_t u2 = ((seq_t)u * (seq_t)u) % (seq_t)m;   /* U(m)^2 mod m */
            if (u2 == (seq_t)1) {
                cache_append(m);
            }
        }
        next_candidate += 2;   /* next odd number */
    }
    return cache[n-1];        /* 1‑based indexing */
}
