/*
 * A285315  Numbers n for which A019565(n) < n.
 * https://oeis.org/A285315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th m >= 1 such that A019565(m) < m (search; A019565(m)
 *            = Product_{k in bits of m} prime(k+1), with early cutoff)
 * Range:     u8 n <= 13, u16 n <= 488, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9829 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static const uint32_t A285315_primes[64] = {
    2, 3, 5, 7, 11, 13, 17, 19,
    23, 29, 31, 37, 41, 43, 47, 53,
    59, 61, 67, 71, 73, 79, 83, 89,
    97, 101, 103, 107, 109, 113, 127, 131,
    137, 139, 149, 151, 157, 163, 167, 173,
    179, 181, 191, 193, 197, 199, 211, 223,
    227, 229, 233, 239, 241, 251, 257, 263,
    269, 271, 277, 281, 283, 293, 307, 311
};

static int A285315_less(uint64_t m)
{
    uint64_t lim = m - 1;
    uint64_t r = 1;
    uint64_t x = m;
    int bit = 0;
    while (x) {
        if (x & 1ULL) {
            uint64_t p = A285315_primes[bit];
            if (r > lim / p) return 0;
            r *= p;
        }
        x >>= 1;
        ++bit;
    }
    return r < m;
}

static uint64_t *A285315_terms = 0;
static uint64_t A285315_count = 0;
static uint64_t A285315_cap = 0;
static uint64_t A285315_next = 1;

seq_t a(idx_t n)
{
    while (A285315_count < (uint64_t)n) {
        uint64_t m = A285315_next++;
        if (A285315_less(m)) {
            if (A285315_count == A285315_cap) {
                A285315_cap = A285315_cap ? A285315_cap * 2 : 1024;
                A285315_terms = (uint64_t *)realloc(A285315_terms, A285315_cap * sizeof(uint64_t));
            }
            A285315_terms[A285315_count++] = m;
        }
    }
    return (seq_t)A285315_terms[n - 1];
}
