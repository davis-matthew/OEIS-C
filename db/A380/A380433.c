/*
 * A380433  Numbers that are a sum of both four and six consecutive prime
 *          numbers.
 * https://oeis.org/A380433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes: a(n) = the n-th number that can be expressed both as the sum
   of 4 consecutive primes and as the sum of 6 consecutive primes, sorted
   increasingly.  The implementation rebuilds the data set only when the
   requested index exceeds the currently known terms. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/build_primes_upto.h"

/* ---------- static state ------------------------------------------------- */
static idx_t        g_limit      = 2000000;     /* current prime bound     */
static uint64_t    *g_primes     = NULL;        /* primes up to g_limit    */
static size_t       g_prime_cnt  = 0;           /* number of primes        */
static seq_t       *g_seq        = NULL;        /* intersection list       */
static size_t       g_seq_cnt    = 0;           /* number of stored terms  */
static size_t       g_seq_cap    = 0;           /* current array capacity  */
static bool         g_initialized = false;      /* first call done?       */

/* ---------- helper: rebuild all tables ----------------------------------- */
static void recompute(void)
{
    /* free existing buffers */
    if (g_primes) free(g_primes);
    if (g_seq)    free(g_seq);

    /* build primes up to g_limit */
    build_primes_upto(g_limit, &g_primes, &g_prime_cnt);

    /* At least 6 primes are needed */
    if (g_prime_cnt < 6) {
        g_seq_cnt = 0;
        g_seq_cap = 0;
        return;
    }

    /* number of possible sums of 4 and 6 consecutive primes */
    size_t sums4_len = g_prime_cnt - 3;
    size_t sums6_len = g_prime_cnt - 5;

    /* temporary buffers for the sums */
    seq_t *sums4 = (seq_t*)malloc(sizeof(seq_t) * sums4_len);
    seq_t *sums6 = (seq_t*)malloc(sizeof(seq_t) * sums6_len);
    if (!sums4 || !sums6) {
        /* allocation failure – keep previous state */
        return;
    }

    /* compute sums of 4 consecutive primes */
    for (size_t i = 0, j = 0; j < sums4_len; ++i, ++j) {
        seq_t sum = (seq_t)g_primes[i] + (seq_t)g_primes[i+1]
                 + (seq_t)g_primes[i+2] + (seq_t)g_primes[i+3];
        sums4[j] = sum;
    }

    /* compute sums of 6 consecutive primes */
    for (size_t i = 0, j = 0; j < sums6_len; ++i, ++j) {
        seq_t sum =  (seq_t)g_primes[i] + (seq_t)g_primes[i+1]
                  + (seq_t)g_primes[i+2] + (seq_t)g_primes[i+3]
                  + (seq_t)g_primes[i+4] + (seq_t)g_primes[i+5];
        sums6[j] = sum;
    }

    /* reserve space for the intersection – its size ≤ min(...) */
    g_seq_cap = sums4_len < sums6_len ? sums4_len : sums6_len;
    g_seq = (seq_t*)malloc(sizeof(seq_t) * g_seq_cap);
    if (!g_seq) {
        free(sums4);
        free(sums6);
        return;
    }
    g_seq_cnt = 0;

    /* two‑pointer traversal to get the intersection */
    size_t i = 0, j = 0;
    while (i < sums4_len && j < sums6_len) {
        seq_t v4 = sums4[i];
        seq_t v6 = sums6[j];
        if (v4 == v6) {
            g_seq[g_seq_cnt++] = v4;
            ++i; ++j;
        } else if (v4 < v6) {
            ++i;
        } else {
            ++j;
        }
    }

    /* clean up temporary buffers */
    free(sums4);
    free(sums6);
}

/* ---------- public routine ------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;      /* undefined, but return 0 */

    if (!g_initialized) {
        recompute();
        g_initialized = true;
    }

    /* enlarge the prime bound until enough terms are present */
    while ((idx_t)g_seq_cnt < n) {
        g_limit *= 2;
        recompute();
    }

    /* safe: n-1 ≤ g_seq_cnt-1 (checked above) */
    return g_seq[(size_t)(n-1)];
}
