/*
 * A331808  Composite numbers of the form 2*p + q, where p, q are consecutive
 *          prime numbers.
 * https://oeis.org/A331808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth composite number of the form 2*p + q, where p and
 *            q are consecutive primes, listed in increasing order
 * Range:     u8 n <= 14, u16 n <= 2078, u32 n <= 2281, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"      /* bool is_prime(idx_t n)          */
#include "../utils/is_prime_big.h"  /* bool is_prime_big(seq_t n)      */

/* ---------- static state for primes and sequence values ---------- */
static uint64_t *primes   = NULL;   /* array of primes up to last found   */
static size_t   p_cnt    = 0;       /* number of primes stored            */
static size_t   p_cap    = 0;       /* allocated capacity of primes array */
static uint64_t last_prime = 1;     /* last prime added, start before 2 */

/* array for composite terms of the sequence */
static seq_t *vals = NULL;
static size_t v_cnt = 0;            /* number of stored terms         */
static size_t v_cap = 0;            /* allocated capacity of vals array */

static idx_t next_pair_idx = 0;     /* index of the first prime of the current pair */

/* ---------- helper functions ---------- */
static void grow_primes(void)
{
    if (p_cnt == p_cap) {
        p_cap = (p_cap == 0) ? 16 : p_cap * 2;
        primes = (uint64_t *)realloc(primes, p_cap * sizeof(*primes));
    }
}

static void grow_vals(void)
{
    if (v_cnt == v_cap) {
        v_cap = (v_cap == 0) ? 16 : v_cap * 2;
        vals = (seq_t *)realloc(vals, v_cap * sizeof(*vals));
    }
}

static void add_next_prime(void)
{
    uint64_t cand = last_prime + 1;
    while (!is_prime(cand))
        ++cand;
    grow_primes();
    primes[p_cnt++] = cand;
    last_prime = cand;
}

static void ensure_primes(size_t need)
{
    while (p_cnt < need)
        add_next_prime();
}

/* ---------- the required function ---------- */
seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  For safety, return 0 for n==0. */
    if (n == 0)
        return (seq_t)0;

    /* Generate terms until we have at least n of them. */
    while (v_cnt < (size_t)n) {
        /* Ensure there are enough primes to form the next pair. */
        ensure_primes(next_pair_idx + 2);

        uint64_t p = primes[next_pair_idx];
        uint64_t q = primes[next_pair_idx + 1];

        /* Candidate = 2*p + q; use seq_t for arbitrary precision. */
        seq_t candidate = ((seq_t)2) * (seq_t)p + (seq_t)q;

        /* Store only if composite. */
        if (!is_prime_big(candidate)) {
            grow_vals();
            vals[v_cnt++] = candidate;
        }
        ++next_pair_idx;
    }

    return vals[n - 1];
}
