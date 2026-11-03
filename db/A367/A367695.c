/*
 * A367695  Numbers k such that k and k+1 are both exponentially odd numbers
 *          (A268335).
 * https://oeis.org/A367695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 124, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 7558 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* k is exponentially odd iff every exponent of its factorization is odd
   (1 counts); the terms are found in order by trial division and cached.
   (The original sized its prime table as limit/10 + 10, too small for the
   first limits, and searched past the primes it had sieved.) */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

static bool is_exponentially_odd(uint64_t n)
{
    for (uint64_t p = 2; p * p <= n; ++p)
        if (n % p == 0) {
            unsigned c = 0;
            while (n % p == 0) { n /= p; ++c; }
            if (!(c & 1)) return false;
        }
    return true;                              /* what is left is 1 or a prime */
}

seq_t a(idx_t n)
{
    static seq_t *A = NULL;
    static size_t len = 0, cap = 0;
    static uint64_t next = 1;                 /* the next k to test */
    static bool next_odd = true;              /* is_exponentially_odd(next) */
    if (n == 0) return (seq_t)0;
    while (len < (size_t)n) {
        bool succ = is_exponentially_odd(next + 1);
        if (next_odd && succ) {
            if (len == cap) {
                cap = cap ? 2 * cap : 1024;
                seq_t *t = mem_resize(seq_t, A, cap);
                if (!t) abort();
                A = t;
            }
            A[len++] = (seq_t)next;
        }
        ++next;
        next_odd = succ;
    }
    return A[n - 1];
}
