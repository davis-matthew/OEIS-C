/*
 * A381869  Smallest starting prime for which the sum of 2*n consecutive
 *          primes is 0 modulo 10, or -1 if no such prime exists.
 * https://oeis.org/A381869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 3289, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Brute‑force search for OEIS A381869: smallest starting prime p such that
   the sum of 2*n consecutive primes starting at p is 0 (mod 10), returning
   -1 if no such prime exists. */

/* Function prototype already defined by the build system:
   seq_t a(idx_t n); */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/next_prime.h"

static seq_t *primes = NULL;      /* list of primes */
static size_t prime_cap = 0, prime_cnt = 0;

static uint8_t *pref_mod10 = NULL; /* prefix sums of primes modulo 10,
                                      pref_mod10[0] = 0, pref_mod10[i+1] = (sum of first i primes) % 10 */
static size_t pref_cap = 0;

/* Ensure capacity for at least 'need' primes and corresponding prefix entries. */
static void ensure_capacity(size_t need)
{
    if (need <= prime_cap) return;
    size_t newcap = prime_cap ? prime_cap << 1 : 16;
    while (newcap < need) newcap <<= 1;
    primes = (seq_t*)realloc(primes, newcap * sizeof(seq_t));
    pref_mod10 = (uint8_t*)realloc(pref_mod10, (newcap + 1) * sizeof(uint8_t));
    prime_cap = newcap;
    pref_cap = newcap + 1;
}

/* Append the next prime after the current last prime, updating the prefix.
   The primes are small enough that start can be represented as uint64_t. */
static void append_next_prime(void)
{
    uint64_t start = (prime_cnt == 0) ? 2
                 : (uint64_t)(primes[prime_cnt - 1]) + 1;
    uint64_t nxt = next_prime(start);
    seq_t val = (seq_t)nxt;

    ensure_capacity(prime_cnt + 1);
    primes[prime_cnt] = val;

    /* Compute prefix modulo 10 for the new prime. */
    uint8_t rem = (uint8_t)(val % (seq_t)10);
    pref_mod10[prime_cnt + 1] = (pref_mod10[prime_cnt] + rem) % 10;

    prime_cnt++;
}

/* Build at least 'len' primes (state is kept between calls). */
static void build_until_prime_count(size_t len)
{
    while (prime_cnt < len) append_next_prime();
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)-1;      /* undefined for offset 0 */

    size_t window = (size_t)2 * (size_t)n;          /* 2*n consecutive primes */

    /* First ensure we have at least one full window. */
    build_until_prime_count(window);

    /* Scan existing windows. */
    for (size_t i = 0; i + window <= prime_cnt; ++i) {
        uint8_t diff = (pref_mod10[i + window] + 10 - pref_mod10[i]) % 10;
        if (diff == 0) return primes[i];
    }

    /* If not found yet, keep generating primes and testing only the newest window. */
    while (1) {
        append_next_prime();
        size_t start = prime_cnt - window;           /* start index of new window */

        if (start >= prime_cnt) break;               /* safety, should not happen */

        uint8_t diff = (pref_mod10[start + window]
                        + 10 - pref_mod10[start]) % 10;
        if (diff == 0) return primes[start];
    }

    /* Unreachable, but keep compiler happy. */
    return (seq_t)-1;
}
