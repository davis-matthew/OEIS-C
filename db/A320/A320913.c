/*
 * A320913  Numbers with an even number of prime factors (counted with
 *          multiplicity) that cannot be factored into squarefree semiprimes
 *          (A320891) but can be factored into distinct semiprimes (A320912).
 * https://oeis.org/A320913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 2747, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, is_graphical
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A320913: numbers with an even number of prime factors (counted with multiplicity) that cannot be factored into square‑free semiprimes but can be factored into distinct semiprimes.
   The function computes terms by sieving natural numbers and testing the required properties. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/factor_u64.h"
#include "../utils/is_graphical.h"

static seq_t *seq = NULL;     /* array of sequence values */
static size_t seq_len = 0;    /* number of values stored */
static size_t seq_cap = 0;    /* capacity of seq array */
static uint64_t next_candidate = 1;   /* next integer to test */

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* Helper: determine whether n is a member of A320913 */
static bool is_candidate(uint64_t n)
{
    if (n < 2) return false;               /* 1 is not included */

    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);
    if (cnt == 0) return false;           /* should not happen */

    /* total number of prime factors and maximum exponent */
    unsigned int omega = 0, max_e = 0;
    for (size_t i = 0; i < cnt; ++i) {
        omega += (unsigned int)exps[i];
        if (exps[i] > max_e) max_e = (unsigned int)exps[i];
    }
    if (omega & 1) return false;           /* odd number of prime factors */

    if (max_e <= omega / 2) return false; /* can be factored into squarefree semiprimes */

    /* indices of primes whose exponent is at least 2 (possible loops) */
    size_t loop_idx[64];
    size_t loop_k = 0;
    for (size_t i = 0; i < cnt; ++i)
        if (exps[i] >= 2) loop_idx[loop_k++] = i;

    unsigned int base_deg[64];
    for (size_t i = 0; i < cnt; ++i) base_deg[i] = (unsigned int)exps[i];

    uint64_t total_masks = 1ULL << loop_k;   /* iterate over all loop selections */
    for (uint64_t mask = 0; mask < total_masks; ++mask) {
        unsigned int d[64];
        memcpy(d, base_deg, cnt * sizeof(unsigned int));

        /* apply chosen loops: subtract 2 from selected primes */
        for (size_t j = 0; j < loop_k; ++j)
            if (mask & (1ULL << j))
                d[loop_idx[j]] -= 2;

        /* check bounds and total degree parity */
        unsigned int sumD = 0;
        bool bad = false;
        for (size_t i = 0; i < cnt; ++i) {
            if (d[i] > (unsigned int)(cnt - 1)) { bad = true; break; }
            sumD += d[i];
        }
        if (bad || (sumD & 1)) continue;

        if (is_graphical(d, cnt)) return true;   /* found valid distinct‑semiprime factorisation */
    }
    return false;
}

/* ------------------------------------------------------------------ */
/* Grow the sequence storage to accommodate at least 'needed' elements */
static void grow_to(size_t needed)
{
    if (seq_cap >= needed) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 64;
    while (new_cap < needed) new_cap *= 2;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Append a value to the sequence array */
static void append_value(uint64_t val)
{
    grow_to(seq_len + 1);
    seq[seq_len++] = (seq_t)val;
}

/* ------------------------------------------------------------------ */
/* Main sequence function required by the test harness */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for index 0 */
    while (seq_len < (size_t)n) {
        while (!is_candidate(next_candidate)) ++next_candidate;
        append_value(next_candidate);
        ++next_candidate;
    }
    return seq[(size_t)n - 1];
}
