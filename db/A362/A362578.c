/*
 * A362578  Prime numbers followed by two consecutive numbers which are
 *          products of four distinct primes (or tetraprimes).
 * https://oeis.org/A362578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 24, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-03)
 * Helpers:   is_prime_u64, is_squarefree, omega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A362578: a(n) is the nth prime p such that
 * p+1 and p+2 are products of four distinct primes (tetraprimes). */
/* The implementation builds the sequence on demand, memoizing terms. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/is_squarefree.h"
#include "../utils/omega.h"
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

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* static cache for the sequence terms */
static seq_t *terms   = NULL;
static size_t terms_len = 0;
static size_t terms_cap = 0;
static idx_t next_candidate = 2;

/* ensure the cache can hold at least new_cap elements */
static void ensure_terms_cap(size_t new_cap)
{
    if (new_cap <= terms_cap)
        return;
    size_t new_cap_act = terms_cap ? terms_cap << 1 : 16;
    if (new_cap_act < new_cap)
        new_cap_act = new_cap;
    seq_t *tmp = seq_realloc(terms, terms_cap, new_cap_act);
    if (!tmp)
        abort();   /* out‑of‑memory */
    terms = tmp;
    terms_cap = new_cap_act;
}

/* append a new term to the cache */
static void push_term(seq_t val)
{
    ensure_terms_cap(terms_len + 1);
    terms[terms_len++] = val;
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1 */

    /* generate terms until we have at least n of them */
    while (terms_len < n) {
        /* skip even numbers greater than 2 */
        if (next_candidate > 2 && (next_candidate & 1ULL) == 0) {
            ++next_candidate;
            continue;
        }

        if (is_prime_big((seq_t)next_candidate)) {
            uint64_t a1 = (uint64_t)next_candidate + 1;
            uint64_t a2 = (uint64_t)next_candidate + 2;

            if (is_squarefree((seq_t)a1) &&
                omega(a1) == 4 &&
                is_squarefree((seq_t)a2) &&
                omega(a2) == 4) {
                push_term((seq_t)next_candidate);
            }
        }
        ++next_candidate;
    }

    return terms[n - 1];
}
