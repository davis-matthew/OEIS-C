/*
 * A323577  Primes p such that 2 is a primitive root modulo p while 2048 is
 *          not.
 * https://oeis.org/A323577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime p such that 2 is a primitive root modulo p
 *            and p ≡ 1 (mod 11). This implementation searches primes
 *            sequentially, keeps a memoised list of found terms, and stops
 *            once the requested index has been reached
 * Range:     u8 n <= 1, u16 n <= 217, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 62 terms through n = 9754 (2026-11-03)
 * Helpers:   factorize, is_prime, mem, next_prime, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include <string.h>
#include "../utils/factorize.h"
#include "../utils/is_prime.h"
#include "../utils/next_prime.h"
#include "../utils/powmod.h"
#include "../utils/mem.h"

/* Helper to grow a sequence array of type seq_t. */
static void grow_seq_array(seq_t **arr, size_t *size, size_t new_len)
{
    if (*size >= new_len) return;
    size_t new_cap = (*size > 0) ? (*size << 1) : 16;
    while (new_cap < new_len) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
    if (!tmp) abort();
    *arr = tmp;
    *size = new_cap;
}

/* Check if 2 is a primitive root modulo the odd prime p. */
static bool is_primitive_root_two(uint64_t p)
{
    uint64_t phi = p - 1;
    uint64_t primes[64];
    int exps[64];
    size_t cnt = factorize(phi, primes, exps);   /* distinct prime factors of φ(p) */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t q = primes[i];
        /* If 2^((p-1)/q) ≡ 1 (mod p), 2 is not a primitive root. */
        if (powmod((seq_t)2, phi / q, (seq_t)p) == (seq_t)1)
            return false;
    }
    return true;
}

/* Return the n‑th term of the sequence.  n is 1‑based. */
seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static size_t seq_cnt   = 0;   /* number of terms currently stored */
    static size_t seq_cap   = 0;   /* current capacity of seq array */
    static uint64_t last_checked = 1;   /* last prime considered by next_prime */

    /* Allocate and grow the array to hold at least n terms. */
    if (seq == NULL) {
        seq_cap = 64;
        seq = mem_new(seq_t, seq_cap);
    }
    if ((size_t)n > seq_cap) {
        size_t new_cap = seq_cap;
        if (new_cap == 0) new_cap = 64;
        while (new_cap < (size_t)n) new_cap *= 2;
        grow_seq_array(&seq, &seq_cap, new_cap);
    }

    /* Generate additional terms until we have the n‑th one. */
    while (seq_cnt < (size_t)n) {
        uint64_t cand = next_prime(last_checked);
        last_checked = cand;

        if (cand % 11 != 1) continue;          /* must satisfy p ≡ 1 (mod 11) */
        if (cand == 2) continue;               /* 2 is not ≡ 1 (mod 11) anyway */

        if (is_primitive_root_two(cand)) {
            seq[seq_cnt++] = (seq_t)cand;
        }
    }

    return seq[n - 1];
}
