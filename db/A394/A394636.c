/*
 * A394636  Start of exactly two even numbers that are totient numbers
 *          (A002202).
 * https://oeis.org/A394636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth even integer x such that (i) x and x+2 are
 *            totient numbers, (ii) x-2 and x+4 are NOT totient numbers. The
 *            (i) and (ii) conditions are checked by the totient sieve
 * Range:     u8 n <= 6, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* static state */
static idx_t  sieve_limit = 0;          /* current total sieve bound          */
static bool   *is_totient = NULL;       /* boolean array of totients           */
static seq_t  *seq_array  = NULL;       /* generated sequence                  */
static size_t seq_len    = 0;           /* number of terms stored              */
static size_t seq_cap    = 0;           /* capacity of seq_array               */

/* free the current totient and sequence data */
static void free_data(void)
{
    if (is_totient) {
        free(is_totient);
        is_totient = NULL;
    }
    if (seq_array) {
        free(seq_array);
        seq_array = NULL;
    }
    seq_len = seq_cap = 0;
}

/* recompute totient membership up to limit `lim` (lim>=2) */
static void recompute_totients(idx_t lim)
{
    free_data();

    /* allocate boolean array for totients */
    is_totient = (bool*)calloc((size_t)lim + 1U, sizeof(bool));
    if (!is_totient) abort();

    /* allocate array for Euler's phi values */
    uint64_t *phi = (uint64_t*)calloc((size_t)lim + 1U, sizeof(uint64_t));
    if (!phi) abort();

    /* initialise phi[i] = i */
    for (size_t i = 0; i <= (size_t)lim; ++i) phi[i] = i;

    /* sieve to compute phi */
    for (size_t i = 2; i <= (size_t)lim; ++i) {
        if (phi[i] == i) {              /* i is prime */
            for (size_t j = i; j <= (size_t)lim; j += i)
                phi[j] -= phi[j] / i;
        }
    }

    /* mark which numbers are totients */
    for (size_t i = 1; i <= (size_t)lim; ++i) {
        uint64_t val = phi[i];
        if (val <= lim) {
            is_totient[(size_t)val] = true;
        }
    }
    free(phi);

    /* build the sequence up to the new limit */
    for (size_t x = 4; x + 4 <= (size_t)lim; ++x) {
        if ((x & 1U) != 0U) continue;            /* x must be even          */
        if (!is_totient[x]) continue;
        if (!is_totient[x + 2U]) continue;
        if (is_totient[x - 2U]) continue;
        if (is_totient[x + 4U]) continue;

        /* grow seq_array if needed */
        if (seq_len == seq_cap) {
            size_t new_cap = (seq_cap == 0U) ? 16U : seq_cap * 2U;
            seq_t *tmp = (seq_t*)realloc(seq_array, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            seq_array = tmp;
            seq_cap   = new_cap;
        }
        seq_array[seq_len++] = (seq_t)x;
    }
}

/* ensure that at least `n` terms of the sequence are calculated */
static void ensure_terms(idx_t n)
{
    while (seq_len < n) {
        if (sieve_limit == 0ULL)
            sieve_limit = (idx_t)(1ULL << 16);   /* 65536  */
        else
            sieve_limit <<= 1ULL;                /* double */
        recompute_totients(sieve_limit);
    }
}

seq_t a(idx_t n)
{
    if (n < 1ULL) return 0;
    ensure_terms(n);
    return seq_array[n - 1U];
}
