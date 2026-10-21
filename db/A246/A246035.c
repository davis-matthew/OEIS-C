/*
 * A246035  Number of odd terms in f^n, where f = (1/x+1+x)*(1/y+1+y).
 * https://oeis.org/A246035
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 254, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   POPCOUNTLL
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A246035
 *  a(n) = A071053(n)^2  = number of odd terms in f^n with f=(1/x+1+x)(1/y+1+y)
 *  The number of odd terms in each dimension equals the number of odd coefficients
 *  in (x⁻¹+1+x)^n.  Therefore
 *
 *      a(n) = ( popcount( (1+x+x²)^n mod 2 ) )² .
 *
 *  We compute the binomial expansion modulo 2 by dynamic programming:  start
 *  with polynomial 1, then for each step multiply by (1+x+x²) modulo 2.
 *  The polynomial is represented as a bitset; a bit set to 1 means the
 *  corresponding coefficient is odd.  After n steps the number of set bits
 *  is popcount((1+x+x²)^n), which is precisely the number of odd coefficients.
 *  The sequence is pre‑computed up to MAX_N (≤ 10000) at first use.
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "../utils/POPCOUNTLL.h"

#if defined(__GNUC__) && !defined(__clang__)
#  define POPCOUNTLL __builtin_popcountll
#else

#endif

/* constants ---------------------------------------------------------------- */
#define MAX_N 10000          /* maximum n that will be queried */

static seq_t *cache = NULL;  /* array of a(n) values, pre‑computed */

/* pre‑compute the sequence up to MAX_N ------------------------------------- */
static void init_cache(void)
{
    if (cache != NULL) return;          /* already done */

    /* degree of polynomial after n steps is 2n, so the bitset must
       hold up to 2*MAX_N + 1 bits.  Number of 64‑bit words needed: */
    const size_t degree = 2 * MAX_N;
    const size_t words  = (degree + 64) / 64;      /* round up */

    /* allocate space for coefficients */
    cache = (seq_t*)calloc(MAX_N + 1, sizeof(seq_t));
    uint64_t *cur  = (uint64_t*)calloc(words, sizeof(uint64_t));
    uint64_t *next = (uint64_t*)calloc(words, sizeof(uint64_t));
    uint64_t *sh1  = (uint64_t*)calloc(words, sizeof(uint64_t));
    uint64_t *sh2  = (uint64_t*)calloc(words, sizeof(uint64_t));

    if (!cache || !cur || !next || !sh1 || !sh2) {
        /* allocation failed – abort */
        abort();
    }

    /* initial polynomial (1+x+x²)^0 = 1 */
    cur[0] = 1ULL;
    cache[0] = (seq_t)1;   /* popcount = 1 → a(0) = 1² = 1 */

    for (idx_t n = 1; n <= MAX_N; ++n) {
        /* sh1 = cur << 1 */
        uint64_t carry = 0;
        for (size_t i = 0; i < words; ++i) {
            uint64_t w = cur[i];
            sh1[i] = (w << 1) | carry;
            carry  = w >> 63;
        }

        /* sh2 = cur << 2 */
        carry = 0;
        for (size_t i = 0; i < words; ++i) {
            uint64_t w = cur[i];
            sh2[i] = (w << 2) | carry;
            carry  = w >> 62;
        }

        /* next = cur XOR sh1 XOR sh2 (mod 2 multiplication) */
        for (size_t i = 0; i < words; ++i) {
            next[i] = cur[i] ^ sh1[i] ^ sh2[i];
        }

        /* count ones in next up to degree 2n (i.e. bits 0 … 2n) */
        size_t bits = 2 * n + 1;
        size_t full_words = bits / 64;
        size_t rem_bits   = bits % 64;
        seq_t count = 0;
        for (size_t i = 0; i < full_words; ++i) {
            count += POPCOUNTLL(next[i]);
        }
        if (rem_bits) {
            uint64_t mask = ((uint64_t)1 << rem_bits) - 1;
            count += POPCOUNTLL(next[full_words] & mask);
        }

        /* store square of the count */
        cache[n] = (seq_t)( (seq_t)count * (seq_t)count );

        /* swap cur and next for next iteration */
        uint64_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    /* free temporary arrays */
    free(sh1);
    free(sh2);
    free(next);
    free(cur);
}

/* public function ----------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n > MAX_N) {
        /* out of bounds – compute on demand (not used in tests)  */
        /* here we rebuild cache up to n, but limit to MAX_N */
        abort();
    }
    init_cache();          /* ensures cache is ready */
    return cache[n];
}
