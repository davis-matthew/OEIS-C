/*
 * A346027  Primes that are the first in a run of exactly 7 emirps.
 * https://oeis.org/A346027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Primes that are the first in a run of exactly 7 emirps */
/* Implements a(idx_t n) which returns the n-th term (n ≥ 1). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/rev.h"

/* helper: index of an odd number in the bit set */
static inline idx_t bit_index(idx_t n)
{
    /* n is odd ≥ 3, map to 0.. */
    return (n - 3) >> 1;
}

/* Mark odd number n as composite in the bit set. */
static inline void setComposite(idx_t n, unsigned char *comp)
{
    idx_t idx = bit_index(n);
    comp[idx >> 3] |= (1U << (idx & 7));
}

/* Query composite status of odd number n. */
static inline int isComposite(idx_t n, unsigned char *comp)
{
    idx_t idx = bit_index(n);
    return (comp[idx >> 3] >> (idx & 7)) & 1U;
}

/* Test primality using the bit set. */
static inline int isPrimeBitset(idx_t n, unsigned char *comp, idx_t limit)
{
    if (n < 2) return 0;
    if (n == 2) return 1;
    if ((n & 1) == 0) return 0;
    if (n > limit) return 0;          /* out of sieve range → assume composite */
    return !isComposite(n, comp);
}

/* ------------------------------------------------------------------ */
/* Static storage for the sequence values.                            */
/* ------------------------------------------------------------------ */
static seq_t *run_seq   = NULL;   /* array of found start primes           */
static size_t run_cap   = 0;      /* allocated capacity of run_seq        */
static size_t run_cnt   = 0;      /* number of terms stored in run_seq    */
static bool  initialized = false;/* flag whether build_sequence ran      */

/* ------------------------------------------------------------------ */
/* Build the sequence once, storing all run‑start primes up to a      */
/* generous sieve limit.  The limit is chosen large enough to yield  */
/* at least 10 000 terms in typical test cases.                       */
/* ------------------------------------------------------------------ */
static void build_sequence(void)
{
    const idx_t LIMIT = 200000000ULL;          /* 200 M is adequate for n≤10 000 */
    /* Number of odd numbers up to LIMIT (including LIMIT if odd). */
    idx_t odd_count = (LIMIT >> 1) + ((LIMIT & 1) ? 1 : 0);
    size_t comp_bytes = (odd_count + 7) >> 3;   /* ceil(bits/8)   */

    unsigned char *comp = (unsigned char *)calloc(comp_bytes, 1);
    if (!comp) { exit(1); }

    /* Sieve of Eratosthenes on odd numbers only. */
    for (idx_t i = 3; i * i <= LIMIT; i += 2) {
        if (!isComposite(i, comp)) {
            idx_t step = i << 1;           /* 2*i */
            for (idx_t j = i * i; j <= LIMIT; j += step)
                setComposite(j, comp);
        }
    }

    /* Sliding window of the last 9 primes (most recent at position 0). */
    idx_t last_vals[9];
    int   last_flags[9];              /* 1 if emirp, 0 otherwise */
    size_t count = 0;                 /* number of primes seen */

    /* Handle prime 2 first. */
    idx_t p = 2;
    int emirp_flag = 0;                /* 2 is not an emirp */
    last_vals[0] = p; last_flags[0] = emirp_flag;
    count = 1;

    /* Scan odd numbers for primes. */
    for (idx_t n = 3; n <= LIMIT; n += 2) {
        if (!isComposite(n, comp)) {
            p = n;
            idx_t rev_v = rev(p);
            int rev_is_prime = (rev_v != p) && isPrimeBitset(rev_v, comp, LIMIT);
            emirp_flag = rev_is_prime ? 1 : 0;

            /* Shift window by one position. */
            for (int i = 8; i > 0; --i) {
                last_vals[i] = last_vals[i-1];
                last_flags[i] = last_flags[i-1];
            }
            last_vals[0] = p;
            last_flags[0] = emirp_flag;
            ++count;

            if (count >= 9) {
                /* Check pattern: non-emir, 7 emirps, non-emir */
                if (!last_flags[0] && !last_flags[8] &&
                    last_flags[1] && last_flags[2] &&
                    last_flags[3] && last_flags[4] &&
                    last_flags[5] && last_flags[6] &&
                    last_flags[7]) {
                    /* The start prime of the run is the first emirp,
                       i.e., last_vals[7]. */
                    if (run_cnt == run_cap) {
                        run_cap = run_cap ? run_cap * 2 : 1024;
                        seq_t *tmp = (seq_t *)realloc(run_seq,
                                                      run_cap * sizeof(seq_t));
                        if (!tmp) { free(comp); exit(1); }
                        run_seq = tmp;
                    }
                    run_seq[run_cnt++] = (seq_t)last_vals[7];
                }
            }
        }
    }

    free(comp);
    initialized = true;
}

/* ------------------------------------------------------------------ */
/* Public entry point required by the framework.                       */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined for n < 1 */
    if (!initialized) build_sequence();
    if ((size_t)(n-1) >= run_cnt) return (seq_t)0; /* overflow, no value */
    return run_seq[n-1];
}
