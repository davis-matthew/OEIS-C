/*
 * A346416  Primes p such that the greatest perimeter of a triangle with prime
 *          sides including p and the next prime is prime.
 * https://oeis.org/A346416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 486, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9447 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements OEIS sequence A346416.
   For each prime p, let q be the next prime.
   Let r be the largest prime less than p+q.
   If the perimeter P = p + q + r is prime, then p is in the sequence.
   The function builds the sequence lazily, storing the primes in a static
   array, so calls in any order can be answered quickly. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Utility functions supplied by the build system */
#include "../utils/build_primes_upto.h"
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

/* Find the index of the largest prime strictly less than S.
   If there is none, return SIZE_MAX. */
static size_t find_last_prime_lt(uint64_t S, const uint64_t *primes, size_t primes_cnt)
{
    size_t lo = 0, hi = primes_cnt;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (primes[mid] < S)
            lo = mid + 1;
        else
            hi = mid;
    }
    if (lo == 0)
        return SIZE_MAX;
    return lo - 1;
}

/* OEIS function */
seq_t a(idx_t n)
{
    /* Static variables to keep state across calls */
    static uint64_t *primes = NULL;
    static size_t primes_cnt = 0;
    static size_t prime_idx = 0;   /* index of the next prime to process */

    static seq_t *seq = NULL;
    static size_t seq_cnt = 0;
    static size_t seq_cap = 0;

    /* Limit large enough to produce at least ~10 000 terms for the required
       benchmark (≈20 million gives ~1.3 million primes) */
    static const idx_t PRIME_LIMIT = 20000000;

    /* Initialise on first call */
    if (!seq) {
        /* Build the list of primes once */
        size_t cnt = 0;
        uint64_t *tmp = NULL;
        build_primes_upto(PRIME_LIMIT, &tmp, &cnt);
        primes = tmp;
        primes_cnt = cnt;

        /* Sequence storage */
        seq_cap = 128;
        seq = mem_new(seq_t, seq_cap);
        seq_cnt = 0;
        prime_idx = 0;
    }

    /* Produce sequence terms until we have at least n entries */
    while (seq_cnt < (size_t)n) {
        /* Need two consecutive primes */
        if (prime_idx + 1 >= primes_cnt)
            break;  /* no more primes – should not happen for the chosen limit */

        uint64_t p = primes[prime_idx];
        uint64_t q = primes[prime_idx + 1];
        uint64_t S = p + q;            /* p + q <= 2*PRIME_LIMIT */
        size_t r_idx = find_last_prime_lt(S, primes, primes_cnt);
        if (r_idx == SIZE_MAX) {
            /* No prime less than p+q – unlikely with chosen limit */
            prime_idx++;
            continue;
        }
        uint64_t r = primes[r_idx];

        uint64_t perimeter = p + q + r;  /* < 3*PRIME_LIMIT, fits in 32‑bit */

        /* Test primality of the perimeter */
        if (is_prime_big((seq_t)perimeter)) {
            /* Grow sequence storage if necessary */
            if (seq_cnt == seq_cap) {
                seq_cap <<= 1;
                seq = mem_resize(seq_t, seq, seq_cap);
            }
            seq[seq_cnt++] = (seq_t)p;
        }

        prime_idx++;  /* move to the next prime for the next iteration */
    }

    /* The sequence is guaranteed to have at least n elements for the tested
       magnitude of n; otherwise the behaviour is undefined (per the spec). */
    return seq[(size_t)n - 1];
}
