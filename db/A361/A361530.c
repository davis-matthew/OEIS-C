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
 * Validated: standard, pass, 69 terms through n = 6853 (2026-11-03)
 * Helpers:   is_prime_u64, mem, next_prime
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
#include <stdlib.h>
#include "../utils/next_prime.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

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
