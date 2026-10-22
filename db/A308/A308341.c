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
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 6, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   is_prime_big, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Utility headers for dynamic memory and primality */
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/is_prime_big.h"

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
