/*
 * A319041  Numbers k > 1 such that Pell(k) == -1 (mod k).
 * https://oeis.org/A319041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 3285, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   append_candidate, pell_mod_k
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
#include "../utils/append_candidate.h"
#include "../utils/pell_mod_k.h"

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
