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
 * Helpers:   factorize, grow_seq_array, is_prime, mod_pow, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/is_prime.h"
#include "../utils/next_prime.h"
#include "../utils/factorize.h"
#include "../utils/mod_pow.h"
#include "../utils/grow_seq_array.h"

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
        if (mod_pow((seq_t)2, phi / q, (seq_t)p) == (seq_t)1)
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
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
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
