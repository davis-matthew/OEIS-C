/*
 * A361671  Squarefree part of the n-th tetrahedral number.
 * https://oeis.org/A361671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = squarefree part of (n*(n+1)*(n+2)/6)
 * Range:     u8 n <= 10, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation factors n, n+1 and n+2, removes the single
 * factors of 2 and 3 (division by 6), keeps track of the parity of
 * each prime's exponent, and returns the product of the primes that
 * appear an odd number of times.  The prime list is pre‑computed once
 * up to a fixed bound that comfortably covers the n values
 * encountered in the tests. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes.h"

#define MAX_PRIME 20000   /* >= maximum possible prime factor for n <= 10000 */

static idx_t *primes = NULL;          /* list of primes up to MAX_PRIME */
static size_t primes_cnt = 0;
static bool primes_ready = false;

static void init_primes(void)
{
    if (!primes_ready) {
        build_primes((idx_t)MAX_PRIME, &primes, &primes_cnt);
        primes_ready = true;
    }
}

/* helper tables for parity bookkeeping */
static unsigned char parity[MAX_PRIME + 1];
static bool seen[MAX_PRIME + 1];
static idx_t used_primes[MAX_PRIME + 1];
static size_t used_cnt = 0;

/* toggle the parity of prime p and record it once per call */
static void toggle_prime(idx_t p)
{
    if (p <= MAX_PRIME) {
        if (!seen[p]) {
            seen[p] = true;
            used_primes[used_cnt++] = p;
        }
        parity[p] ^= 1;
    }
}

/* compute the squarefree part from the parity table */
static seq_t compute_from_parity(void)
{
    seq_t res = 1;
    for (size_t i = 0; i < used_cnt; ++i) {
        idx_t p = used_primes[i];
        if (parity[p])
            res = res * (seq_t)p;
    }
    return res;
}

/* clear parity and seen tables for next call */
static void reset_parity(void)
{
    for (size_t i = 0; i < used_cnt; ++i) {
        idx_t p = used_primes[i];
        parity[p] = 0;
        seen[p] = false;
    }
    used_cnt = 0;
}

seq_t a(idx_t n)
{
    init_primes();

    /* factor n, n+1, n+2 one by one */
    idx_t nums[3] = { n, n + 1, n + 2 };

    for (int k = 0; k < 3; ++k) {
        idx_t val = nums[k];
        for (size_t i = 0; i < primes_cnt; ++i) {
            idx_t p = primes[i];
            if (p * p > val)
                break;
            int cnt = 0;
            while (val % p == 0) {
                val /= p;
                ++cnt;
            }
            if (cnt & 1)   /* odd exponent in this number */
                toggle_prime(p);
        }
        if (val > 1) {          /* remaining prime factor */
            toggle_prime(val);
        }
    }

    /* remove the single factors of 2 and 3 (division by 6) */
    toggle_prime(2);
    toggle_prime(3);

    seq_t result = compute_from_parity();
    reset_parity();

    return result;
}
