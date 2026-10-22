/*
 * A361232  Numbers m such that the increasing sequence of divisors of m,
 *          regarded as words on the finite alphabet of its prime factors, is
 *          ordered lexicographically.
 * https://oeis.org/A361232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer m (m ≥ 1) satisfying that for its prime *
 *            factorisation p_1^e_1 ⋯ p_r^e_r (with p_1 < … < p_r) we have *
 *            p_i > ∏_{j < i} p_j^e_j for all i = 1 … r. * * Implementation
 *            details: * - Precompute the array of smallest prime factors
 *            (spf) up to a * fixed limit (2 000 000) by a si...
 * Range:     u8 n <= 203, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/* Smallest prime factor table (global, lazily initialised).             */
/* --------------------------------------------------------------------- */
static uint32_t *spf = NULL;   /* spf[i] = smallest prime factor of i   */
static const idx_t SPF_LIMIT = 2000000;   /* sufficient for the problem */

static void init_spf(void) {
    if (spf != NULL) return;   /* already initialised */
    spf = (uint32_t *)malloc((SPF_LIMIT + 1) * sizeof(uint32_t));
    if (spf == NULL) abort();  /* out of memory */
    for (idx_t i = 0; i <= SPF_LIMIT; ++i) spf[i] = (uint32_t)i;
    spf[0] = 0; spf[1] = 1;
    for (idx_t i = 2; i * i <= SPF_LIMIT; ++i) {
        if (spf[i] == (uint32_t)i) {          /* i is prime            */
            for (idx_t j = i * i; j <= SPF_LIMIT; j += i)
                if (spf[j] == (uint32_t)j)
                    spf[j] = (uint32_t)i;
        }
    }
}

/* --------------------------------------------------------------------- */
/* Test whether a given integer m satisfies the lexicographic condition. */
/* --------------------------------------------------------------------- */
static bool is_lexicographic(uint64_t m) {
    if (m <= 1) return true;   /* m = 1 always satisfies */

    uint64_t tmp = m;
    uint64_t cumulative = 1;   /* product of all prime powers processed so far */

    while (tmp > 1) {
        uint32_t p = spf[ (idx_t)tmp ];
        uint64_t power = 1;
        int exp   = 0;

        /* factor out all occurrences of prime p */
        while (tmp % p == 0) {
            tmp /= p;
            ++exp;
            power *= p;
        }

        /* check the inequality p > cumulative                 */
        if ((uint64_t)p <= cumulative) return false;

        /* update cumulative = cumulative * p^exp              */
        if (cumulative > UINT64_MAX / power) {
            /* overflow would mean m is larger than our sieve limit;
               this situation cannot happen for the intended inputs.  */
            return false;
        }
        cumulative *= power;
    }

    return true;
}

/* --------------------------------------------------------------------- */
/* Sequence storage.  Capacity 20000 is more than enough for n ≤ 10000.  */
/* --------------------------------------------------------------------- */
static seq_t seq_values[20000];   /* static array – works for integer and big‑int types */
static size_t seq_len = 0;        /* number of values currently stored   */
static uint64_t last_checked = 0; /* last integer tested for inclusion    */

/* --------------------------------------------------------------------- */
/* Public function: returns a(n) respecting OEIS's offset of 1           */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n) {
    static bool init_done = false;
    if (!init_done) {
        init_spf();
        init_done = true;
    }

    if (n == 0) return 0;      /* the sequence is defined for n ≥ 1   */
    while (seq_len < (size_t)n) {
        ++last_checked;
        if (is_lexicographic(last_checked)) {
            if (seq_len < sizeof(seq_values)/sizeof(seq_values[0])) {
                seq_values[seq_len] = (seq_t)last_checked;
                ++seq_len;
            } else {
                /* Extremely rare situation – we run out of preallocated
                   space.  Since the requirements only ask for n up to
                   ~10 000, this branch is never executed in practice.  */
                abort();
            }
        }
    }
    return seq_values[n - 1];
}
