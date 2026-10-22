/*
 * A316524  Signed sum over the prime indices of n.
 * https://oeis.org/A316524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 718, i16 n <= 8710, i32 n <= 8710, i64 n <= 8710,
 *            i128 n <= 8710, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A316524 – Signed sum over the prime indices of n.
    For n = Π p_i (counted with multiplicity, primes sorted ascending),
    a(n) = Σ_{i=1}^{k} (-1)^{i-1} * index(p_i),  
    where index(p) is the 1‑based position of prime p in the list
    2,3,5,7,11,….  Example:  n = 12 = 2²·3 → indices (1,1,2) → a(12)=1‑1+2=2.
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* ===== Dynamically built list of primes (ascending) ===== */

static uint32_t *primes = NULL;   /* array of primes up to last computed limit */
static size_t primes_cnt = 0;     /* number of primes stored */
static size_t primes_cap = 0;     /* capacity of primes array */

/* Ensure that all primes up to 'limit' are generated and stored in 'primes'.
   Uses a simple incremental prime generation algorithm. */
static void ensure_primes_upto(idx_t limit)
{
    if (limit <= primes_cnt ? primes[primes_cnt - 1] : 0)
        return;                     /* already have all needed primes */

    uint32_t start = (primes_cnt == 0) ? 2 : primes[primes_cnt - 1] + 1;
    for (uint32_t candidate = start; candidate <= (uint32_t)limit; ++candidate) {
        bool is_prime = true;
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint32_t p = primes[i];
            if ((uint64_t)p * p > candidate)
                break;
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (primes_cnt == primes_cap) {
                size_t new_cap = primes_cap ? primes_cap * 2 : 8;
                uint32_t *tmp = (uint32_t *)realloc(primes, new_cap * sizeof(uint32_t));
                if (!tmp) abort();       /* allocation failed – terminate */
                primes = tmp;
                primes_cap = new_cap;
            }
            primes[primes_cnt++] = candidate;
        }
    }
}

/* Find the 1‑based index of a prime 'p' (which must be in the primes array). */
static size_t prime_index(uint32_t p)
{
    /* linear search is fine for the size we handle (≤ ~1229 primes for n≤10000). */
    for (size_t i = 0; i < primes_cnt; ++i)
        if (primes[i] == p)
            return i + 1;
    return 0; /* should not happen for valid inputs */
}

/* ===== Main OEIS sequence function ===== */

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    /* generate primes up to n to allow index lookup of any factor ≤ n */
    ensure_primes_upto(n);

    idx_t m = n;
    seq_t sum = 0;
    int sign = 1;          /* +1 or -1 */

    /* Process primes up to sqrt(m) (dynamic because m shrinks) */
    for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * primes[i] <= m; ++i) {
        uint32_t p = primes[i];
        if (m % p == 0) {
            /* count multiplicity of prime factor p */
            int exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            size_t idx = i + 1; /* prime indices are 1‑based */
            for (int k = 0; k < exp; ++k) {
                sum += sign * (seq_t)idx;
                sign = -sign;
            }
        }
    }

    /* If remaining m > 1, it is a prime factor with exponent 1 */
    if (m > 1) {
        /* m ≤ n, hence its index exists in the primes array */
        size_t idx = prime_index((uint32_t)m);
        sum += sign * (seq_t)idx;
        /* sign updated if further factors existed (not needed) */
    }

    return sum;
}
