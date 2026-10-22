/*
 * A365242  Numbers k such that k+1, k-1, k/2+1, k/2-1, k/3+1 and k/3-1 are
 *          all prime.
 * https://oeis.org/A365242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   is_prime_big
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
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n); */

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
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
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
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_len++] = k;
        }

        /* Advance the chosen residue class for next iteration */
        m_vals[best_i] += 2310ULL;
    }

    return seq[n-1];
}
