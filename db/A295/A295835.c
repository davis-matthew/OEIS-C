/*
 * A295835  Numbers k == 3 (mod 4) such that 2^((k-1)/2), 3^((k-1)/2) and
 *          5^((k-1)/2) are congruent to 1 (mod k).
 * https://oeis.org/A295835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 414, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   jacobi_symbol, mod_pow, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A295835: Numbers k ≡ 3 (mod 4) with 2^((k‑1)/2), 3^((k‑1)/2) and 5^((k‑1)/2) ≡ 1 (mod k). */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/mod_pow.h"
#include "../utils/jacobi_symbol.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* Helper: test whether a candidate k satisfies the condition. */
static bool good_k(idx_t k)
{
    /* Quickly filter with Jacobi symbols. */
    if (jacobi_symbol(2, (uint64_t)k) != 1) return false;
    if (jacobi_symbol(3, (uint64_t)k) != 1) return false;
    if (jacobi_symbol(5, (uint64_t)k) != 1) return false;

    uint64_t m = ((uint64_t)k - 1) / 2;
    seq_t mod = (seq_t)k;
    seq_t one = 1;

    if (mod_pow((seq_t)2, m, mod) != one) return false;
    if (mod_pow((seq_t)3, m, mod) != one) return false;
    if (mod_pow((seq_t)5, m, mod) != one) return false;

    return true;
}

/* External-function sequence entry point. */
seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static size_t  seq_len = 0;
    static size_t  seq_cap = 0;
    static idx_t   next_k  = 3;          /* start from the first 3 mod 4 candidate */

    if (n == 0) return 0;               /* invalid index, but safe return */

    /* Ensure the table contains at least n terms. */
    if (seq_len < (size_t)n) {
        if (!seq) {
            seq_cap = 256;              /* initial capacity */
            seq = seq_alloc(seq_cap);
            seq_len = 0;
        }

        while (seq_len < (size_t)n) {
            /* Find the next valid candidate. */
            while (!good_k(next_k)) {
                next_k += 4;
            }

            /* Grow the array if necessary. */
            if (seq_len == seq_cap) {
                size_t new_cap = seq_cap * 2;
                seq_t *tmp = seq_realloc(seq, seq_cap, new_cap);
                seq = tmp;
                seq_cap = new_cap;
            }

            seq[seq_len++] = (seq_t)next_k;   /* store the found term */
            next_k += 4;                      /* next 3 mod 4 candidate */
        }
    }

    return seq[n - 1];
}
