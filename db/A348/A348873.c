/*
 * A348873  After a(1) = 1, the sequence is always extended with the smallest
 *          divisor d (not yet present in the sequence) of the last term t. If
 *          d doesn't exist, we extend the sequence with 3*t + 1 and repeat.
 *          See the Comments section for more details.
 * https://oeis.org/A348873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   factor_u64, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(1) = 1.
   For n > 1, let t = a(n‑1). Find the smallest divisor d of t that
   has not appeared earlier in the sequence. If such d exists,
   a(n) = d; otherwise a(n) = 3*t + 1. */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "../utils/is_prime_big.h"
#include "../utils/factor_u64.h"

static seq_t *seq      = NULL;
static size_t seq_len  = 0;
static size_t seq_cap  = 0;

/* Check whether a value has already appeared in the sequence. */
static bool present(seq_t val)
{
    for (size_t i = 0; i < seq_len; ++i)
        if (seq[i] == val)
            return true;
    return false;
}

/* Recursively generate all divisors of t (excluding 1 and t) and keep the
   smallest divisor that has not yet appeared in the sequence. */
static void gen_divs(const uint64_t *pr, const uint64_t *ex,
                     size_t pfcnt, size_t idx, uint64_t curr, uint64_t t,
                     seq_t *best, bool *found)
{
    if (idx == pfcnt) {
        if (curr > 1 && curr < t) {
            seq_t d = curr;
            if (!present(d)) {
                if (!*found || d < *best) {
                    *best = d;
                    *found = true;
                }
            }
        }
        return;
    }
    uint64_t p = pr[idx];
    uint64_t v = curr;
    for (uint64_t i = 0; i <= ex[idx]; ++i) {
        gen_divs(pr, ex, pfcnt, idx + 1, v, t, best, found);
        v *= p;
    }
}

/* Main function returning the nth term (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;        /* offset not used in the sequence */

    size_t target = (size_t)n;

    if (seq_len == 0) {   /* initialise with a(1) = 1 */
        seq_cap = 16;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq[0] = (seq_t)1;
        seq_len = 1;
    }

    while (seq_len < target) {
        seq_t t = seq[seq_len - 1];
        seq_t nxt;

        if (t == (seq_t)1 || is_prime_big(t)) {
            nxt = t * (seq_t)3 + (seq_t)1;
        } else {
            uint64_t primes[64];
            uint64_t exps[64];
            size_t pfcnt = 0;

            factor_u64((uint64_t)t, primes, exps, &pfcnt);

            bool found = false;
            seq_t best = 0;
            gen_divs(primes, exps, pfcnt, 0, (uint64_t)1, (uint64_t)t, &best, &found);

            if (found)
                nxt = best;
            else
                nxt = t * (seq_t)3 + (seq_t)1;
        }

        if (seq_len == seq_cap) {
            seq_cap *= 2;
            seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
        }
        seq[seq_len++] = nxt;
    }

    return seq[target - 1];
}
