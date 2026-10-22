/*
 * A366846  Numbers whose odd prime indices are relatively prime.
 * https://oeis.org/A366846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th smallest positive integer whose odd prime
 *            indices are relatively prime. Implementation: enumerate integers
 *            in ascending order, test the property by prime‐factorisation
 *            using a growing list of primes. The test succeeds if the GCD of
 *            all odd prime indices (index counted starting fro...
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* ---------- prime list handling ------------------------------------------ */
static uint64_t *prime_list = NULL;   /* primes in ascending order */
static size_t  prime_cnt   = 0;       /* number of primes stored    */
static size_t  prime_cap   = 0;       /* allocated capacity         */

/* ensure primes up to at least 'up_to' (inclusive). */
static void ensure_primes(uint64_t up_to)
{
    if (prime_cnt && prime_list[prime_cnt - 1] >= up_to)
        return;

    uint64_t start = prime_cnt ? prime_list[prime_cnt - 1] + 1 : 2;
    for (uint64_t cand = start; cand <= up_to; ++cand) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_list[i];
            if (p * p > cand) break;
            if (cand % p == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            if (prime_cnt >= prime_cap) {
                prime_cap = prime_cap ? prime_cap * 2 : 64;
                prime_list = (uint64_t *)realloc(prime_list,
                                                prime_cap * sizeof(uint64_t));
            }
            prime_list[prime_cnt++] = cand;
        }
    }
}

/* binary search to get the 1‑based index of prime 'p' in prime_list. */
static idx_t prime_index(uint64_t p)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] < p) lo = mid + 1;
        else if (prime_list[mid] > p) hi = mid;
        else return (idx_t)(mid + 1);   /* indices are 1‑based in the OEIS data */
    }
    return 0;   /* should not happen for primes that are in the list */
}

/* Euclidean gcd for 32‑bit unsigned values (used for indices). */
static uint32_t gcd_u32(uint32_t a, uint32_t b)
{
    while (b != 0) {
        uint32_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* ---------- property test for a number ------------------------------------ */
static bool good_number(uint64_t n)
{
    if (n == 1) return false;     /* 1 has no prime factors */

    uint64_t temp = n;
    bool hasOdd = false;
    uint32_t g = 0;               /* GCD of odd indices encountered */

    /* ensure primes up to sqrt(temp) for factorisation: safe to extend up to temp */
    ensure_primes(temp);

    for (size_t i = 0; i < prime_cnt && (uint64_t)prime_list[i] * prime_list[i] <= temp; ++i) {
        uint64_t p = prime_list[i];
        if (temp % p == 0) {
            uint32_t idx = (uint32_t)prime_index(p);   /* 1‑based index */
            if (idx & 1) {                            /* odd index */
                if (!hasOdd) {
                    g = idx;
                    hasOdd = true;
                } else {
                    g = gcd_u32(g, idx);
                    if (g == 1) return true;   /* early exit */
                }
            }
            while (temp % p == 0) temp /= p;          /* remove all copies of p */
        }
    }

    if (temp > 1) {   /* remaining prime factor */
        uint32_t idx = (uint32_t)prime_index(temp);
        if (idx & 1) {
            if (!hasOdd) {
                g = idx;
                hasOdd = true;
            } else {
                g = gcd_u32(g, idx);
                if (g == 1) return true;
            }
        }
    }

    return hasOdd && g == 1;
}

/* ---------- main sequence function ---------------------------------------- */
seq_t a(idx_t n)
{
    /* the sequence starts at n = 1 */
    if (n == 0) return (seq_t)0;

    uint64_t count = 0;
    uint64_t candidate = 1;

    while (count < n) {
        /* some candidates may exceed current prime list, extend it */
        ensure_primes(candidate);

        if (good_number(candidate)) {
            ++count;
        }
        ++candidate;
    }

    return (seq_t)(candidate - 1);
}
