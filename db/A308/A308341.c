/*
 * A308341  Hypotenuses of primitive Pythagorean triangles two sides of which
 *          are Pythagorean primes.
 * https://oeis.org/A308341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   The sequence A308341 consists of the hypotenuses c = 2*m^2 + 2*m
 *            + 1 of primitive Pythagorean triples (2*m+1, 2*m^2+2*m, c) for
 *            integers m ≥ 2 such that - 2*m+1 is prime (an odd leg that is a
 *            Pythagorean prime) - c is prime - for m > 2, m*(m+1) is
 *            divisible by 30 (ensuring the longer leg is 60 ti...
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 36 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Utility headers for dynamic memory and primality */
#include <stdlib.h>
#include "../utils/is_prime_u64.h"

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
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

static seq_t *seq   = NULL;   /* cache of sequence terms          */
static size_t seq_cap = 0;    /* allocated capacity of seq        */
static size_t seq_len = 0;    /* number of terms already stored   */
static uint64_t m_last  = 1;  /* last m processed (previous)      */

/* Ensure the sequence cache has at least needed capacity */
static void ensure_seq_capacity(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 16;
    while (new_cap < needed) new_cap <<= 1;
    if (seq == NULL) {
        seq = seq_alloc(new_cap);
    } else {
        seq = seq_realloc(seq, seq_cap, new_cap);
    }
    seq_cap = new_cap;
}

/* Main function: returns the n-th term of the sequence (a(1) is first) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined below offset 1 */

    /* Generate new terms until we reach the requested index.
       The generation is idempotent and works for any call order. */
    while (seq_len < (size_t)n) {
        ++m_last;                      /* candidate m */
        /* m must be even (so 2*m+1 ≡ 1 mod 4) except for m=2 */
        if (m_last > 2 && (m_last & 1)) continue;

        /* For m > 2 the longer leg 2*m^2+2*m must be divisible by 60,
           i.e., m*(m+1) ≡ 0 (mod 30). */
        if (m_last > 2 && (m_last * (m_last + 1) % 30 != 0)) continue;

        /* 2*m+1 must be prime */
        seq_t odd_leg = (seq_t)2 * (seq_t)m_last + 1;
        if (!is_prime_big(odd_leg)) continue;

        /* Compute the hypotenuse c = 2*m^2 + 2*m + 1 */
        seq_t m_s   = (seq_t)m_last;
        seq_t hyp   = 2 * m_s * m_s + 2 * m_s + (seq_t)1;
        if (!is_prime_big(hyp)) continue;

        /* Store the found term */
        ensure_seq_capacity(seq_len + 1);
        seq[seq_len++] = hyp;
    }
    return seq[n - 1];
}
