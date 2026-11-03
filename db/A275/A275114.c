/*
 * A275114  Primes p for which the sum of the numbers in the Collatz iteration
 *          (A033493) of p is a prime.
 * https://oeis.org/A275114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-03)
 * Helpers:   is_prime_seq, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A275114 : primes p such that the sum of the
 * Collatz trajectory of p is a prime number.  The function does
 * not use a closed formula but searches primes and tests the
 * Collatz sum on demand, memoising results.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_seq.h"
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

static seq_t *seq_list = NULL;   /* stored primes whose Collatz sum is prime */
static size_t seq_len  = 0;      /* number of stored terms */
static size_t seq_cap  = 0;      /* array capacity */

static uint64_t cand = 1;        /* next integer to test for primality */

/* Ensure seq_list can hold at least 'min_cap' elements */
static void ensure_seq_cap(size_t min_cap)
{
    if (seq_cap < min_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 4;
        while (new_cap < min_cap) new_cap *= 2;
        seq_t *tmp = mem_resize(seq_t, seq_list, new_cap);
        if (!tmp) abort();          /* out of memory */
        seq_list = tmp;
        seq_cap  = new_cap;
    }
}

/* Extend seq_list until it contains at least 'target' terms */
static void extend_to(idx_t target)
{
    while (seq_len < (size_t)target) {
        /* Find next prime candidate */
        do { ++cand; } while (!is_prime_seq(cand));

        /* Compute the sum of the Collatz trajectory of this prime */
        seq_t sum = (seq_t)cand;
        seq_t x   = (seq_t)cand;
        while (x != (seq_t)1) {
            if ((x % (seq_t)2) == (seq_t)0) {
                x = x / (seq_t)2;
            } else {
                x = (seq_t)3 * x + (seq_t)1;
            }
            sum += x;
        }

        /* If the sum is prime, record the prime itself */
        if (is_prime_big(sum)) {
            ensure_seq_cap(seq_len + 1);
            seq_list[seq_len++] = (seq_t)cand;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* outside the OEIS offset */
    extend_to(n);
    return seq_list[n - 1];
}
