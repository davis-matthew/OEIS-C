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
 * Validated: standard, pass, 62 terms through n = 9459 (2026-11-01)
 * Helpers:   powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A295835: Numbers k ≡ 3 (mod 4) with 2^((k‑1)/2), 3^((k‑1)/2) and 5^((k‑1)/2) ≡ 1 (mod k). */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/powmod.h"

/* Jacobi symbol (a|n) for odd positive n <= 2^64-1.
   Returns 1, 0, or -1. */
static int jacobi_symbol(uint64_t a, uint64_t n) {
    if (n == 0) return 0;
    a %= n;
    if (a == 0) return 0;
    int res = 1;
    while (a != 0) {
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t n_mod8 = n & 7;
            if (n_mod8 == 3 || n_mod8 == 5)
                res = -res;
        }
        uint64_t tmp = a;
        a = n;
        n = tmp;
        if ((a & 3) == 3 && (n & 3) == 3)
            res = -res;
        a %= n;
    }
    return (n == 1) ? res : 0;
}

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

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

    if (powmod((seq_t)2, m, mod) != one) return false;
    if (powmod((seq_t)3, m, mod) != one) return false;
    if (powmod((seq_t)5, m, mod) != one) return false;

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
