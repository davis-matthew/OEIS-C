/*
 * A327877  Numbers having an odd number of non-unitary prime factors.
 * https://oeis.org/A327877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9979 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers having an odd number of non‑unitary prime factors.
   A number's non‑unitary prime factors are the primes that appear
   with exponent ≥ 2 in its prime factorisation. This routine
   generates the sequence in increasing order and returns the nth term
   (1‑based).  It memoises the terms in a static array so that calls
   may appear in any order. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static bool has_odd_nonunitary(uint64_t x)
{
    uint64_t t = x;
    int odd_cnt = 0;

    /* handle factor 2 */
    if ((t & 1ULL) == 0) {
        int e = 0;
        while ((t & 1ULL) == 0) {
            t >>= 1;
            e++;
        }
        if (e >= 2) odd_cnt ^= 1;
    }

    /* factor odd primes */
    for (uint64_t p = 3; p <= t / p; p += 2) {
        if (t % p == 0) {
            int e = 0;
            while (t % p == 0) {
                t /= p;
                e++;
            }
            if (e >= 2) odd_cnt ^= 1;
        }
    }

    /* any remaining t > 1 is a prime with exponent 1 – doesn't affect count */
    return odd_cnt == 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined but harmless */

    static seq_t *seq          = NULL;
    static size_t seq_cap      = 0;
    static idx_t seq_len       = 0;
    static uint64_t cur         = 2;          /* next integer to test */

    if (!seq) {
        seq_cap = 64;
        seq = mem_new(seq_t, seq_cap);
    }

    while (seq_len < n) {
        if (has_odd_nonunitary(cur)) {
            if (seq_len == seq_cap) {
                seq_cap <<= 1;
                seq = mem_resize(seq_t, seq, seq_cap);
            }
            seq[seq_len++] = (seq_t)cur;
        }
        cur++;
    }

    return seq[n - 1];   /* a(1) is stored at seq[0] */
}
