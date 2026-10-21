/*
 * A244365  Table read by rows: row n contains all primes p such that prime(n)
 *          < p <= floor(prime(n)^(1+1/n)).
 * https://oeis.org/A244365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 151, u16 n <= 6401, u32 n <= 6401, u64 n <= 6401,
 *            u128 n <= 6401, big any n
 * Helpers:   append_candidate, ensure_capacity, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A244365: Table read by rows
   Row n contains all primes p such that prime(n) < p ≤ floor(prime(n)^(1+1/n)).
   The flattened table gives a(1)=3, a(2)=5, … .
   This implementation builds the table on demand, memoises the
   resulting sequence, and generates primes on the fly. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/is_prime_small.h"
#include "../utils/append_candidate.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    /* Static state for memoisation and for prime generation */
    static seq_t *seq = NULL;        /* cached sequence values */
    static size_t seq_len = 0, seq_cap = 0;

    static uint64_t *primes = NULL;  /* list of primes found so far */
    static size_t primes_len = 0, primes_cap = 0;
    static uint64_t last_checked = 1;   /* last integer tested for primality */
    static idx_t current_row = 1;        /* next row number to process (1‑based) */

    /* Ensure at least one slot in the sequence array */
    if (seq_cap == 0) {
        ensure_capacity(1, &seq_cap, (void **)&seq, sizeof(seq_t));
    }

    /* Generate rows until we have at least n elements in the sequence */
    while (seq_len < n) {
        /* Make sure we have the current_row‑th prime to start the row */
        while (primes_len < current_row) {
            do { ++last_checked; } while (!is_prime_small(last_checked));
            if (primes_cap == 0) {
                ensure_capacity(1, &primes_cap, (void **)&primes, sizeof(uint64_t));
            }
            ensure_capacity(primes_len + 1, &primes_cap, (void **)&primes, sizeof(uint64_t));
            primes[primes_len++] = last_checked;
        }

        uint64_t pn = primes[current_row - 1];

        /* Compute upper bound: floor( pn^(1+1/current_row) ) */
        long double exponent = ((long double)(current_row + 1)) /
                               ((long double)current_row);
        long double val = powl((long double)pn, exponent);
        uint64_t bound = (uint64_t)floorl(val + 1e-12L);

        /* Ensure primes list extends up to the bound */
        while (primes_len == 0 || primes[primes_len - 1] <= bound) {
            do { ++last_checked; } while (!is_prime_small(last_checked));
            if (primes_cap == 0) {
                ensure_capacity(1, &primes_cap, (void **)&primes, sizeof(uint64_t));
            }
            ensure_capacity(primes_len + 1, &primes_cap, (void **)&primes, sizeof(uint64_t));
            primes[primes_len++] = last_checked;
        }

        /* Append the primes of this row to the sequence */
        idx_t idx = current_row;          /* first prime index > pn */
        while (idx < primes_len && primes[idx] <= bound) {
            seq_t val = (seq_t)primes[idx];
            append_candidate(&seq, &seq_len, &seq_cap, val);
            ++idx;
        }

        ++current_row;                    /* move to the next row */
    }

    return seq[n - 1];
}
