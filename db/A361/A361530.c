/*
 * A361530  Primes that can be written as the result of shuffling the decimal
 *          digits of two primes.
 * https://oeis.org/A361530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 3775, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate, is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: For each prime, test whether it can be obtained by shuffling
   the decimal digits of two primes.  Primes are generated with next_prime,
   each candidate is examined with can_be_shuffled, and the valid primes are
   cached.  The function a(n) returns the n‑th term of the sequence,
   indexing from 1. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"
#include "../utils/next_prime.h"
#include "../utils/append_candidate.h"

static seq_t   *cache  = NULL;
static size_t   cap    = 0;
static size_t   stored = 0;
static uint64_t cur_p  = 2;   /* next prime to test */

static bool
can_be_shuffled(uint64_t p)
{
    /* Extract digits into an array in most‑significant‑first order. */
    uint8_t dig[20];
    int dlen = 0;
    uint64_t tmp = p;
    while (tmp != 0) {
        dig[dlen++] = (uint8_t)(tmp % 10);
        tmp /= 10;
    }
    for (int i = 0; i < dlen / 2; ++i) {
        uint8_t t = dig[i];
        dig[i] = dig[dlen - 1 - i];
        dig[dlen - 1 - i] = t;
    }

    unsigned int max_mask = (1u << dlen) - 1u;   /* exclude full mask */
    for (unsigned int mask = 1; mask < max_mask; ++mask) {
        seq_t a = 0, b = 0;
        bool leadingA = true, leadingB = true;
        bool bad = false;
        for (int i = 0; i < dlen; ++i) {
            if (mask & (1u << i)) {
                if (leadingA) {
                    if (dig[i] == 0) { bad = true; break; }
                    leadingA = false;
                }
                a = a * 10 + dig[i];
            } else {
                if (leadingB) {
                    if (dig[i] == 0) { bad = true; break; }
                    leadingB = false;
                }
                b = b * 10 + dig[i];
            }
        }
        if (bad) continue;
        if (is_prime_big(a) && is_prime_big(b)) return true;
    }
    return false;
}

seq_t
a(idx_t n)
{
    while (stored < (size_t)n) {
        if (can_be_shuffled(cur_p))
            append_candidate(&cache, &stored, &cap, (seq_t)cur_p);
        cur_p = next_prime(cur_p + 1);
    }
    return cache[(size_t)(n - 1)];
}
