/*
 * A276818  Composite numbers n such that b^gpf(n) == b (mod n) for every
 *          integer b, where gpf(n) = A006530(n).
 * https://oeis.org/A276818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by search: composite squarefree numbers n such that for every prime
   divisor p of n, (p-1) divides gpf(n)-1. */

/* Required includes for types and helper functions */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* Static storage for the generated sequence */
static seq_t *seq = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;
static uint64_t upper_seen = 0;

/* Ensure the dynamic array has room for at least 'needed' elements */
static void ensure_seq_cap(size_t needed)
{
    if (seq_cap >= needed)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(1);               /* allocation failure */
    seq = tmp;
    seq_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)         /* n>=1 by specification; guard anyway */
        return (seq_t)0;
    ensure_seq_cap((size_t)n);
    if (seq_len < (size_t)n) {
        uint64_t cand = upper_seen + 1;
        while (seq_len < (size_t)n) {
            uint64_t primes[64];
            uint64_t exps[64];
            size_t cnt = 0;
            factor_u64(cand, primes, exps, &cnt);

            if (cnt >= 2) {                /* must be composite */
                bool squarefree = true;
                for (size_t i = 0; i < cnt; ++i) {
                    if (exps[i] > 1) {
                        squarefree = false;
                        break;
                    }
                }
                if (!squarefree) {
                    ++cand;
                    continue;
                }

                uint64_t gpf = primes[cnt - 1];
                bool ok = true;
                for (size_t i = 0; i < cnt; ++i) {
                    if ((gpf - 1) % (primes[i] - 1) != 0) {
                        ok = false;
                        break;
                    }
                }

                if (ok) {
                    ensure_seq_cap(seq_len + 1);
                    seq[seq_len++] = (seq_t)cand;
                }
            }
            ++cand;
        }
        upper_seen = cand - 1;
    }
    return seq[n - 1];
}
