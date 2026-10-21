/*
 * A274188  Number n such that there is a smaller positive number j == n (mod
 *          5) such that sqrt(j*n) is an integer.
 * https://oeis.org/A274188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 3733, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A274188 – n such that there exists j<n, j≡n (mod 5),
   and sqrt(j n) is an integer. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h"
#include "../utils/build_primes_upto.h"

/* Square‑free part of n: product of primes dividing n with odd exponent.
   Uses a dynamically grown list of primes. */
static uint64_t *primes = NULL;      /* array of primes */
static size_t prime_cnt = 0;         /* number of primes in the array   */
static idx_t  prime_limit = 0;       /* largest prime that the array covers */

static seq_t squarefree_part(seq_t n)
{
    seq_t result = 1;
    if (n <= 1) return result;

    /* Ensure that primes cover up to sqrt(n) */
    seq_t limit_seq = floor_sqrt(n);
    idx_t limit = (idx_t)limit_seq;
    if (limit > prime_limit) {
        uint64_t *newp = NULL;
        size_t newcnt = 0;
        /* build_primes_upto allocates the array; we free the old one. */
        build_primes_upto(limit, &newp, &newcnt);
        free(primes);
        primes = newp;
        prime_cnt = newcnt;
        prime_limit = limit;
    }

    seq_t temp = n;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (p > (uint64_t)limit) break;
        int parity = 0;
        while (temp % (seq_t)p == 0) {
            temp /= (seq_t)p;
            parity ^= 1;
        }
        if (parity) result *= (seq_t)p;
    }

    if (temp > 1) {          /* remaining factor is prime (exponent 1) */
        result *= temp;
    }
    return result;
}

/* Test whether a given n belongs to the sequence. */
static bool is_member(seq_t n)
{
    if (n <= 1) return false;

    seq_t d = squarefree_part(n);      /* square‑free part */
    seq_t t = n / d;                  /* t must be a perfect square */
    seq_t s = floor_sqrt(t);          /* s = sqrt(t) */

    /* If d is divisible by 5 we need at least two squares (s>=2)
       to find a smaller j. */
    if (d % 5 == 0) {
        if (s >= 2) return true;
    }

    /* If s>=6 we can always take j derived from s-5. */
    if (s >= 6) return true;

    /* s is small (1..5); brute‑force search for a suitable s' < s. */
    int dmod5 = (int)(d % 5);
    unsigned int si = (unsigned int)s;   /* s <= 5 here */
    for (unsigned int sp = 1; sp < si; ++sp) {
        int diff = (int)(s * s - (seq_t)sp * (seq_t)sp);  /* s^2 - s'^2 */
        int diffmod5 = diff % 5;
        if ((dmod5 * diffmod5) % 5 == 0) return true;
    }
    return false;
}

/* Main sequence generator.  a(1) is the first member. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined offset */

    seq_t m = 1;                   /* candidate number */
    idx_t count = 0;

    while (1) {
        if (is_member(m)) {
            ++count;
            if (count == n) return m;
        }
        ++m;
    }
}
