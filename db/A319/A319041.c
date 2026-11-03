/*
 * A319041  Numbers k > 1 such that Pell(k) == -1 (mod k).
 * https://oeis.org/A319041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 3285, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Validated: standard, pass, 70 terms through n = 9775 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th term (with a(1)=3) of A319041:
   the positive integers k>1 such that Pell(k) ≡ -1 (mod k).
   The implementation searches for k in increasing order and
   memoises the found values so that subsequent calls are fast. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

/* Fast computation of Pell(n) modulo k via matrix exponentiation
   of [[2,1],[1,0]]^n.  The second column of the resulting matrix
   gives Pell(n) in the lower entry.  All intermediate arithmetic
   is performed modulo k using 128‑bit intermediates. */
static inline seq_t pell_mod_k(uint64_t k)
{
    if (k == 1)
        return (seq_t)0;

    uint64_t r00 = 1, r01 = 0, r10 = 0, r11 = 1;   /* identity matrix */
    uint64_t m00 = 2 % k, m01 = 1 % k, m10 = 1 % k, m11 = 0 % k; /* base matrix */
    uint64_t n = k;

    while (n) {
        if (n & 1) {
            uint64_t t00 = ((unsigned __int128)r00 * m00 +
                            (unsigned __int128)r01 * m10) % k;
            uint64_t t01 = ((unsigned __int128)r00 * m01 +
                            (unsigned __int128)r01 * m11) % k;
            uint64_t t10 = ((unsigned __int128)r10 * m00 +
                            (unsigned __int128)r11 * m10) % k;
            uint64_t t11 = ((unsigned __int128)r10 * m01 +
                            (unsigned __int128)r11 * m11) % k;
            r00 = t00; r01 = t01; r10 = t10; r11 = t11;
        }
        uint64_t t00 = ((unsigned __int128)m00 * m00 +
                        (unsigned __int128)m01 * m10) % k;
        uint64_t t01 = ((unsigned __int128)m00 * m01 +
                        (unsigned __int128)m01 * m11) % k;
        uint64_t t10 = ((unsigned __int128)m10 * m00 +
                        (unsigned __int128)m11 * m10) % k;
        uint64_t t11 = ((unsigned __int128)m10 * m01 +
                        (unsigned __int128)m11 * m11) % k;
        m00 = t00; m01 = t01; m10 = t10; m11 = t11;
        n >>= 1;
    }

    return (seq_t)r10;   /* Pell(n) modulo k */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    static seq_t *terms   = NULL;
    static size_t term_len = 0;
    static size_t term_cap = 0;
    static uint64_t next_k = 2;   /* we will test k = next_k+1 on first loop */

    if (n == 0)
        return (seq_t)0;      /* out of range, but never used */

    while (term_len < (size_t)n) {
        next_k++;
        uint64_t k = next_k;
        seq_t mod = pell_mod_k(k);

        if (mod == (seq_t)(k - 1))
            append_candidate(&terms, &term_len, &term_cap, (seq_t)k);
    }

    return terms[n - 1];
}
