/*
 * A365242  Numbers k such that k+1, k-1, k/2+1, k/2-1, k/3+1 and k/3-1 are
 *          all prime.
 * https://oeis.org/A365242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 23 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search-based algorithm for A365242
 * Numbers k such that k+1, k-1, k/2+1, k/2-1, k/3+1, k/3-1 are all prime.
 * The algorithm iterates over k=6*m where m ≡ 0,210,630,1680,2100 (mod 2310).
 * It generates the terms progressively and caches them for fast repeated access. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
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

seq_t a(idx_t n) {
    /* 1‑based indexing, a(1)=12 is handled separately */
    static seq_t *seq = NULL;        /* cached terms */
    static size_t seq_len = 0;       /* number of terms stored */
    static size_t seq_cap = 0;       /* capacity of seq array */

    /* Handle a(1) explicitly */
    if (n == (idx_t)1)
        return (seq_t)12;

    /* Ensure array has enough capacity */
    if (seq_cap == 0) {
        seq_cap = 64;
        seq = mem_new(seq_t, seq_cap);
    }
    if (seq_len == 0) {         /* store the first term */
        seq[0] = (seq_t)12;
        seq_len = 1;
    }

    /* Small helper to grow the array */
    if (seq_len >= n) return seq[n-1];
    while (seq_len < n) {
        /* Residue classes modulo 2310 for m (k=6*m) that can yield a solution
           after the first term: 0,210,630,1680,2100. */
        static const uint64_t residues[5] = {0ULL, 210ULL, 630ULL, 1680ULL, 2100ULL};
        static int initialized = 0;
        static uint64_t m_vals[5];   /* current offset (m value) for each residue */

        if (!initialized) {
            for (int i = 0; i < 5; ++i) {
                if (residues[i] == 0ULL)
                    m_vals[i] = 2310ULL;   /* skip 0, start at first positive */
                else
                    m_vals[i] = residues[i];
            }
            initialized = 1;
        }

        /* Find the smallest candidate m among the residue classes */
        uint64_t best_m = 0;
        int best_i = 0;
        bool first = true;
        for (int i = 0; i < 5; ++i) {
            uint64_t m = m_vals[i];
            if (first || m < best_m) {
                best_m = m;
                best_i = i;
                first = false;
            }
        }

        /* Candidate k=6*m */
        seq_t k = (seq_t)6 * (seq_t)best_m;

        /* Check the six primality conditions */
        if (is_prime_big(k-1) && is_prime_big(k+1) &&
            is_prime_big((seq_t)3*best_m-1) && is_prime_big((seq_t)3*best_m+1) &&
            is_prime_big((seq_t)2*best_m-1) && is_prime_big((seq_t)2*best_m+1)) {
            /* Store the found term */
            if (seq_len >= seq_cap) {
                seq_cap *= 2;
                seq = mem_resize(seq_t, seq, seq_cap);
            }
            seq[seq_len++] = k;
        }

        /* Advance the chosen residue class for next iteration */
        m_vals[best_i] += 2310ULL;
    }

    return seq[n-1];
}
