/*
 * A297575  Numbers whose sum of divisors is divisible by 10.
 * https://oeis.org/A297575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 1591, u32 n <= 1591, u64 n <= 1591,
 *            u128 n <= 1591, big any n
 * Helpers:   append_candidate, build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find the nth positive integer n whose sum of divisors σ(n) is divisible by 10. */
/* The function scans integers from 1 upward, computing σ(n) mod 10 using a precomputed  
   smallest‑prime‑factor array. When σ(n) ≡ 0 (mod 10) the integer is stored in a dynamic
   array. Subsequent calls reuse the cached data. */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_spf.h"
#include "../utils/append_candidate.h"

static seq_t *seq = NULL;          /* cache of sequence terms   */
static size_t seq_len = 0;         /* number of terms stored    */
static size_t seq_cap = 0;         /* current capacity of seq   */

static idx_t *spf = NULL;         /* smallest prime factor table */
static idx_t spf_limit = 0;        /* maximum index spf covers   */

static idx_t last_candidate = 0;  /* last integer examined      */

/* Ensure the spf table covers at least [0..limit].  If necessary rebuild it. */
static void ensure_spf(idx_t limit)
{
    if (spf_limit >= limit)
        return;

    idx_t new_limit = spf_limit > 0 ? spf_limit * 2 : 1024;
    if (new_limit < limit)
        new_limit = limit;

    idx_t *new_spf = NULL;
    build_spf(new_limit, &new_spf);
    free(spf);
    spf = new_spf;
    spf_limit = new_limit;
}

/* Return the nth term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    /* Grow the cache until it holds at least n terms. */
    while (seq_len < n) {
        /* Advance to the next candidate number. */
        ++last_candidate;
        if (last_candidate == 1)       /* sigma(1) = 1, not acceptable */
            continue;

        /* Ensure we can factor last_candidate. */
        ensure_spf(last_candidate);

        /* Compute σ(last_candidate) modulo 10. */
        idx_t tmp = last_candidate;
        seq_t sigma_mod = 1;  /* product of geometric series modulo 10 */
        while (tmp > 1) {
            idx_t p = spf[tmp];
            uint32_t exp = 0;
            while ((tmp % p) == 0) {
                tmp /= p;
                ++exp;
            }

            /* Sum_{i=0..exp} p^i (mod 10).  Compute multiplicatively. */
            seq_t term = 1 % 10;   /* p^0 */
            seq_t sum  = 1 % 10;   /* initial term */
            seq_t pmod = (seq_t)(p % 10);
            for (uint32_t i = 0; i < exp; ++i) {
                term = (term * pmod) % 10;
                sum  = (sum + term) % 10;
            }
            sigma_mod = (sigma_mod * sum) % 10;
        }

        if (sigma_mod == 0)
            append_candidate(&seq, &seq_len, &seq_cap, (seq_t)last_candidate);
    }

    return seq[n - 1];
}
