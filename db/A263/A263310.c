/*
 * A263310  Numbers n such that p=6*n+1, q=6*p+1 and r=6*q+1 are primes.
 * https://oeis.org/A263310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th integer n such that * p = 6*n + 1, q = 6*p + 1,
 *            r = 6*q + 1 are all primes
 * Range:     u8 none, u16 n <= 9, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 66 terms through n = 924 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function generates terms on demand and memoises the
 * sequence.  It uses deterministic Miller-Rabin for 64‑bit
 * prime testing via is_prime_big. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"
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

static seq_t *sequence = NULL;     /* memoised terms */
static idx_t  count    = 0;         /* number of terms memoised   */
static idx_t  next_min = 1;         /* next candidate to test     */
static size_t capacity = 0;         /* allocated size of sequence  */

/* Append a value to the memoised array, growing it as needed. */
static void append_term(seq_t val)
{
    if (count >= capacity) {
        size_t new_cap = capacity ? capacity * 2 : 16;
        seq_t *new_seq = mem_resize(seq_t, sequence, new_cap);
        if (!new_seq) abort();        /* heap exhaustion */
        sequence = new_seq;
        capacity = new_cap;
    }
    sequence[count++] = val;
}

seq_t a(idx_t n)
{
    /* Offset 1: the first term is a(1).  n==0 is undefined. */
    if (n == 0) return (seq_t)0;

    /* Generate terms until we have at least n terms. */
    while (count < n) {
        seq_t cand = (seq_t)next_min;          /* candidate n            */
        seq_t p    = 6 * cand + 1;             /* p = 6*n + 1           */
        seq_t q    = 6 * p   + 1;              /* q = 6*p + 1           */
        seq_t r    = 6 * q   + 1;              /* r = 6*q + 1           */

        if (is_prime_big(p) && is_prime_big(q) && is_prime_big(r))
            append_term(cand);

        ++next_min;                            /* next candidate to test */
    }

    return sequence[n - 1];
}
