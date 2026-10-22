/*
 * A336377  Primes p(n) such that gcd(n, prime(n)+prime(n+2)) > 1.
 * https://oeis.org/A336377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n-th prime p(n) such that gcd(n, p(n)+p(n+2)) > 1 */
/* Implemented by iteratively generating primes and testing the condition
   for each prime index.  The result is memoized so that subsequent calls
   work in O(1) time per answer (after any required prime generation). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static seq_t *primes = NULL;         /* array of generated primes (p[0] = prime(1)) */
static size_t  prime_cnt = 0, prime_cap = 0;

static seq_t *output = NULL;         /* stored sequence values */
static size_t  out_cnt = 0, out_cap = 0;

static idx_t next_test = 1;          /* next prime index to test */

/* Ensure that primes[0..target-1] exist (i.e. that we have p(1..target)). */
static void
ensure_prime_count(idx_t target)
{
    while (prime_cnt < target) {
        uint64_t cand = 2;
        if (prime_cnt > 0)
            cand = (uint64_t)primes[prime_cnt - 1] + 1U;

        for (;;) {
            bool ok = true;
            for (size_t i = 0; i < prime_cnt; ++i) {
                uint64_t p = (uint64_t)primes[i];
                if (p * p > cand) break;
                if (cand % p == 0) { ok = false; break; }
            }
            if (ok) break;
            ++cand;
        }

        /* Append cand to the primes array. */
        if (prime_cap == 0) {
            prime_cap = 64;
            primes = (seq_t *)malloc(prime_cap * sizeof(seq_t));
        } else if (prime_cnt == prime_cap) {
            prime_cap <<= 1;
            primes = (seq_t *)realloc(primes, prime_cap * sizeof(seq_t));
        }
        primes[prime_cnt++] = (seq_t)cand;
    }
}

seq_t
a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset 1 – unused value */

    /* Grow the sequence until we have at least the n-th element. */
    while (out_cnt < n) {
        /* Make sure we have primes up to index next_test+2 (1‑based). */
        ensure_prime_count(next_test + 2);

        /* Compute gcd(next_test, p(next_test)+p(next_test+2)). */
        seq_t sum = primes[next_test - 1] + primes[next_test + 1];
        seq_t g   = gcd((seq_t)next_test, sum);

        if (g > (seq_t)1) {
            /* Append p(next_test) to the stored sequence. */
            if (out_cap == 0) {
                out_cap = 64;
                output = (seq_t *)malloc(out_cap * sizeof(seq_t));
            } else if (out_cnt == out_cap) {
                out_cap <<= 1;
                output = (seq_t *)realloc(output, out_cap * sizeof(seq_t));
            }
            output[out_cnt++] = primes[next_test - 1];
        }
        ++next_test;
    }
    return output[n - 1];
}
