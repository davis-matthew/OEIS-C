/*
 * A265915  Primes that are sum of 5 consecutive primes congruent to 1 mod 6.
 * https://oeis.org/A265915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) are primes that are the sum of 5 consecutive primes
 *            congruent to 1 mod 6
 * Range:     u8 n <= 3, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation incrementally generates the sequence using on‑the‑fly
 * generation of primes ≡ 1 (mod 6).  Each time a new such prime is found,
 * it keeps the last five and checks whether their sum is prime.  All
 * discovered sums are cached so that future calls to a() can return
 * earlier results without recomputation.  The algorithm runs in
 * essentially linear time in the index requested.  It uses the library
 * function is_prime_big() for primality testing of 64‑bit values.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;          /* cached sequence values */
static size_t cache_count = 0;       /* number of cached values */
static size_t cache_cap = 0;         /* allocated capacity of cache */

/* store all 1 (mod 6) primes found so far */
static uint64_t *primes1 = NULL;
static size_t pcount = 0;
static size_t pcap = 0;

/* next candidate (≡ 1 mod 6) to test for primality */
static uint64_t next_candidate = 7;

/* append a value to the cache, growing storage if necessary */
static void push_cache(seq_t val)
{
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else if (cache_count == cache_cap) {
        cache_cap = cache_cap * 2;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
    cache[cache_count++] = val;
}

/* ensure we have at least m capacity in the primes1 array */
static void ensure_primes1(size_t m)
{
    if (pcap == 0) {
        pcap = 64;
        primes1 = (uint64_t *)malloc(pcap * sizeof(uint64_t));
    } else if (m > pcap) {
        while (pcap < m) pcap <<= 1;
        primes1 = (uint64_t *)realloc(primes1, pcap * sizeof(uint64_t));
    }
}

/* add a 1 (mod 6) prime to the list of primes1 */
static void add_prime1(uint64_t p)
{
    ensure_primes1(pcount + 1);
    primes1[pcount++] = p;
}

/* helper: compute sum of the last five 1 (mod 6) primes using seq_t */
static seq_t sum_last_five(void)
{
    seq_t s = 0;
    for (size_t i = 0; i < 5; ++i) {
        s += (seq_t)primes1[pcount - 5 + i];
    }
    return s;
}

/* main sequence generator: ensure cache has at least n terms */
static void generate_upto(idx_t n)
{
    while (cache_count < (size_t)n) {
        /* generate next 1 (mod 6) prime */
        while (!is_prime_big((seq_t)next_candidate)) {
            next_candidate += 6;
        }
        add_prime1(next_candidate);
        next_candidate += 6;

        if (pcount >= 5) {
            seq_t s = sum_last_five();
            if (is_prime_big(s)) {
                push_cache(s);
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* undefined for n < 1, return 0 as safety */

    generate_upto(n);
    return cache[n - 1];
}
