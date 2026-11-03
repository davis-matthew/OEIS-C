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
 * Validated: standard, pass, 79 terms through n = 6303 (2026-11-03)
 * Helpers:   is_prime_seq, mem
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
#include <stdlib.h>
#include "../utils/is_prime_seq.h"
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#endif

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

/* Grow a dynamic array when needed.  The pointer is passed as (void **). */
#ifdef __cplusplus
template <class T>
static void ensure_capacity(size_t needed, size_t *cap, T **ptr, size_t elem_size) /* elem_size: sizeof(T) */
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    T *tmp = mem_resize(T, *ptr, new_cap);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#else
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#endif

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
        ensure_capacity(1, &seq_cap, OEISC_VPP(&seq), sizeof(seq_t));
    }

    /* Generate rows until we have at least n elements in the sequence */
    while (seq_len < n) {
        /* Make sure we have the current_row‑th prime to start the row */
        while (primes_len < current_row) {
            do { ++last_checked; } while (!is_prime_seq(last_checked));
            if (primes_cap == 0) {
                ensure_capacity(1, &primes_cap, OEISC_VPP(&primes), sizeof(uint64_t));
            }
            ensure_capacity(primes_len + 1, &primes_cap, OEISC_VPP(&primes), sizeof(uint64_t));
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
            do { ++last_checked; } while (!is_prime_seq(last_checked));
            if (primes_cap == 0) {
                ensure_capacity(1, &primes_cap, OEISC_VPP(&primes), sizeof(uint64_t));
            }
            ensure_capacity(primes_len + 1, &primes_cap, OEISC_VPP(&primes), sizeof(uint64_t));
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
