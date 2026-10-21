/*
 * A255485  Number of odd terms in expansion of (1 + x + x^2 + x^4)^n.
 * https://oeis.org/A255485
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 174, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A255485 – Number of odd terms in (1 + x + x^2 + x^4)^n
 *
 *  This implementation builds the coefficient polynomials
 *  modulo 2 iteratively.  Using the identity
 *
 *      (1 + x + x^2 + x^4)^m  (mod 2)
 *
 *  is easy to update by one step:
 *
 *      P(x)_{m+1} = P(x)_m ⊕ P(x)_m·x ⊕ P(x)_m·x^2 ⊕ P(x)_m·x^4
 *
 *  where ⊕ denotes XOR (addition modulo 2).  The number of odd
 *  coefficients equals the Hamming weight of the resulting bit set.
 *  The routine keeps a cache of all values up to the greatest n
 *  requested during execution, so subsequent calls are O(1).
 *
 *  All arithmetic uses only integer operations; no floating point
 *  or external libraries are required.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* ------- static state ------------------------------------- */
    static idx_t cur_n = 0;        /* last n that has been computed */
    static seq_t *a_cache = NULL;  /* a_cache[i] = a(i) for i <= cur_n */
    static size_t a_cache_len = 0;  /* allocated size of a_cache      */

    static uint64_t *bits    = NULL; /* current polynomial bits       */
    static uint64_t *bits_n   = NULL; /* temporary buffer              */
    static size_t  bits_cap  = 0;     /* capacity in words             */
    static size_t  word_cnt  = 0;     /* words actually used (degree+1)*/

    /* ------- initialise on first call --------------------------- */
    if (!a_cache) {
        /* start with a(0)=1 */
        a_cache_len = 2;  /* at least one element for n=0, and room for n=1 */
        a_cache = (seq_t *)malloc(sizeof(seq_t) * a_cache_len);
        if (!a_cache) abort();          /* out of memory */
        a_cache[0] = (seq_t)1;          /* a(0) = 1 */

        /* polynomial for n=0 is just 1 => bit 0 set */
        bits_cap = 1;
        bits  = (uint64_t *)malloc(bits_cap * sizeof(uint64_t));
        bits_n= (uint64_t *)malloc(bits_cap * sizeof(uint64_t));
        if (!bits || !bits_n) abort();
        bits[0]  = 1ULL;
        bits_n[0] = 0ULL;
        word_cnt = 1;
    }

    /* ------- extend cache if needed ----------------------------- */
    while (cur_n < n) {
        /* next exponent will be cur_n + 1 */
        idx_t next = cur_n + 1;
        /* degree increases by 4 each step: new_degree = 4*next */
        idx_t new_degree = 4 * next;
        size_t new_word_cnt = (new_degree >> 6) + 1; /* ceil((deg+1)/64) */

        /* enlarge bit buffers if their capacity is too small */
        if (new_word_cnt > bits_cap) {
            /* double or set to new_word_cnt */
            size_t new_cap = bits_cap * 2;
            if (new_cap < new_word_cnt) new_cap = new_word_cnt;
            uint64_t *tmp = (uint64_t *)realloc(bits, new_cap * sizeof(uint64_t));
            if (!tmp) abort();
            bits = tmp;
            tmp = (uint64_t *)realloc(bits_n, new_cap * sizeof(uint64_t));
            if (!tmp) abort();
            bits_n = tmp;
            bits_cap = new_cap;
        }

        /* initialise destination buffer to zero */
        memset(bits_n, 0, new_word_cnt * sizeof(uint64_t));

        /* copy current polynomial (shift 0) */
        memcpy(bits_n, bits, word_cnt * sizeof(uint64_t));

        /* apply shift 1, 2 and 4 */
        const int shifts[3] = {1, 2, 4};
        for (int s_idx = 0; s_idx < 3; ++s_idx) {
            int shift = shifts[s_idx];
            size_t ws = shift >> 6;     /* word shift */
            int bs = shift & 63;        /* bit shift */

            size_t i_lim = word_cnt;    /* source words count */
            for (size_t i = 0; i < i_lim; ++i) {
                uint64_t v = bits[i];
                if (v == 0) continue;
                size_t dst = i + ws;
                /* lower part */
                bits_n[dst] ^= v << bs;
                /* possible upper part */
                if (bs != 0 && dst + 1 < new_word_cnt) {
                    bits_n[dst + 1] ^= v >> (64 - bs);
                }
            }
        }

        /* update the current polynomial */
        word_cnt = new_word_cnt;
        uint64_t *tmp = bits;
        bits = bits_n;
        bits_n = tmp;

        /* count the number of odd coefficients */
        seq_t cnt = 0;
        for (size_t i = 0; i < word_cnt; ++i) {
            cnt += (seq_t)__builtin_popcountll(bits[i]);
        }

        /* extend a_cache if necessary */
        if ((size_t)next + 1 > a_cache_len) {
            size_t new_len = a_cache_len * 2;
            if (new_len < (size_t)next + 2) new_len = (size_t)next + 2;
            seq_t *tmp = (seq_t *)realloc(a_cache, new_len * sizeof(seq_t));
            if (!tmp) abort();
            a_cache = tmp;
            a_cache_len = new_len;
        }
        a_cache[next] = cnt;

        /* advance current n */
        cur_n = next;
    }

    /* all computed > n? no, so just return */
    return a_cache[n];
}
