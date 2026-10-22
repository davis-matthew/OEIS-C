/*
 * A353217  Triangular numbers (A000217) with arithmetic derivative (A003415)
 *          a palindrome (A002113).
 * https://oeis.org/A353217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 13, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   is_palindrome, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A353217: triangular numbers whose arithmetic derivative is a palindrome. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_palindrome.h"
#include "../utils/seq_realloc.h"

/* Factor a 64‑bit integer n into primes and exponents.  The caller
   supplies arrays big enough to hold all primes.  The number of distinct
   prime factors is returned in *cnt. */
static void factor_uint64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt)
{
    size_t c = 0;
    if (n <= 1) { *cnt = 0; return; }

    while ((n & 1ULL) == 0) {
        ex[c] = 0;  /* will be incremented below */
        do { n >>= 1; ex[c]++; } while ((n & 1ULL) == 0);
        pr[c++] = 2ULL;
    }

    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            ex[c] = 0;
            do { n /= p; ex[c]++; } while (n % p == 0);
            pr[c++] = p;
        }
    }
    if (n > 1) {
        pr[c] = n;
        ex[c] = 1;
        c++;
    }
    *cnt = c;
}

/* Arithmetic derivative of the triangular number T(m) = m*(m+1)/2. */
static seq_t deriv_of_triangular(idx_t m)
{
    if (m <= 1) return 0; /* T(0)=0, T(1)=1 have derivative 0 */

    seq_t t = (seq_t)m * ((seq_t)m + 1) / 2;

    /* Factor m and m+1 */
    uint64_t pr1[64], ex1[64], pr2[64], ex2[64];
    size_t cnt1, cnt2;
    factor_uint64(m, pr1, ex1, &cnt1);
    factor_uint64(m + 1, pr2, ex2, &cnt2);

    /* Merge factorizations */
    uint64_t primes[128], exps[128];
    size_t pc = 0;

    for (size_t i = 0; i < cnt1; ++i) {
        primes[pc] = pr1[i];
        exps[pc]   = ex1[i];
        pc++;
    }
    for (size_t i = 0; i < cnt2; ++i) {
        size_t j, found = 0;
        for (j = 0; j < pc; ++j) {
            if (primes[j] == pr2[i]) { exps[j] += ex2[i]; found = 1; break; }
        }
        if (!found) {
            primes[pc] = pr2[i];
            exps[pc]   = ex2[i];
            pc++;
        }
    }

    /* Remove one factor of 2 due to division by 2. */
    for (size_t j = 0; j < pc; ++j) {
        if (primes[j] == 2ULL) {
            if (exps[j] > 0) exps[j]--;
            break;
        }
    }

    /* Compute derivative = t * Σ(e/p).  Since t/p is integer, we compute
       term = (t / p) * e. */
    seq_t sum = 0;
    for (size_t j = 0; j < pc; ++j) {
        if (exps[j] == 0) continue;
        seq_t term = (t / (seq_t)primes[j]) * (seq_t)exps[j];
        sum += term;
    }
    return sum;
}

/* Cache for the sequence values. */
static seq_t *cache = NULL;
static size_t cache_sz = 0;
static size_t cache_cap = 0;
static idx_t next_m = 0;  /* next m to test */

/* Append a value to cache, expanding storage if necessary. */
static void append_cache(seq_t val)
{
    if (cache_sz >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        seq_t *tmp = seq_realloc(cache, cache_sz, new_cap);
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_sz++] = val;
}

/* Public function: a(n) returns the n-th element of A353217 (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* out of bounds, but harmless */

    while (cache_sz < n) {
        seq_t t = (seq_t)next_m * ((seq_t)next_m + 1) / 2;
        seq_t d = deriv_of_triangular(next_m);
        if (is_palindrome(d, 10))
            append_cache(t);
        next_m++;
    }
    return cache[n - 1];
}
