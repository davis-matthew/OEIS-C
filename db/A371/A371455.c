/*
 * A371455  Numbers k such that if we take the binary indices of each prime
 *          index of k we get an antichain of sets.
 * https://oeis.org/A371455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 158, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n‑th positive integer k such that for every distinct
   prime factor p of k the set of bit positions of the binary
   representation of the prime index of p form an antichain
   (no set is a proper subset of another). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/build_spf.h"

seq_t a(idx_t n) {
    /* static state held across calls */
    static bool initialized = false;
    static idx_t limit = 0;
    static idx_t *spf = NULL;                     /* smallest prime factor table */
    static uint32_t *prime_to_index = NULL;       /* prime → 1‑based index map */
    static seq_t *seq_arr = NULL;                 /* collected valid numbers */
    static size_t seq_len = 0;                    /* number of stored values */
    static size_t seq_cap = 0;                    /* capacity of seq_arr */
    static idx_t next_candidate = 1;              /* next integer to test */

    /*------------------------------------------------------------------
     * First time: build sieve and helper arrays.
     *------------------------------------------------------------------*/
    if (!initialized) {
        /* Choose a sieve limit large enough for n up to about 10 000.
           20 000 000 gives 1 270 607 primes, which is more than enough. */
        limit = 20000000;

        /* Build smallest‑prime‑factor table */
        build_spf(limit, &spf);

        /* Build prime → index mapping (1‑based) */
        prime_to_index = (uint32_t *)calloc(limit + 1, sizeof(uint32_t));
        if (!prime_to_index) return (seq_t)0;    /* allocation failure */
        uint32_t idx = 0;
        for (idx_t i = 2; i <= limit; ++i) {
            if (spf[i] == i) {                  /* i is prime */
                ++idx;
                prime_to_index[i] = idx;
            }
        }

        initialized = true;
    }

    /*------------------------------------------------------------------
     * Generate terms until we have at least n of them.
     *------------------------------------------------------------------*/
    while (seq_len < (size_t)n) {
        /* Safety: stop if we ran out of sieve coverage */
        if (next_candidate > limit)
            return (seq_t)0;                    /* undefined beyond limit */

        bool ok = true;
        idx_t tmp = next_candidate;
        /* masks[i] holds the bit‑position mask for the i‑th distinct prime factor */
        uint64_t masks[64];
        size_t m = 0;

        while (tmp > 1 && ok) {
            idx_t p = spf[tmp];
            /* remove all copies of this prime factor */
            while (tmp % p == 0) tmp /= p;

            uint32_t idxp = prime_to_index[p];          /* 1‑based index of prime p */

            /* compute mask of set bit positions of idxp */
            uint64_t mask = 0;
            int bitpos = 1;
            idx_t val = idxp;
            while (val > 0) {
                if (val & 1U)
                    mask |= (1ULL << (bitpos - 1));
                val >>= 1;
                ++bitpos;
            }

            /* compare with previously seen masks for antichain property */
            for (size_t j = 0; j < m && ok; ++j) {
                uint64_t other = masks[j];
                if ((mask & other) == mask || (mask & other) == other)
                    ok = false;
            }
            if (ok && m < sizeof(masks)/sizeof(masks[0]))
                masks[m++] = mask;
        }

        if (ok) {
            /* store the valid number */
            if (seq_len == seq_cap) {
                seq_cap = seq_cap ? seq_cap * 2 : 32;
                seq_arr = (seq_t *)realloc(seq_arr, seq_cap * sizeof(seq_t));
                if (!seq_arr) return (seq_t)0;    /* allocation failure */
            }
            seq_arr[seq_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }

    /* return the requested term (offset 1) */
    return seq_arr[n - 1];
}
