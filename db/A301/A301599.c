/*
 * A301599  Numbers k at which the ratio r(k) = (k-th prime) / (average of
 *          first k primes) reaches a record high.
 * https://oeis.org/A301599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A301599: record indices of the ratio r(k) = prime(k)·k / Σ₁ᵏ prime(j). */
/* Implemented by generating a small number of primes and keeping the new record positions. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/sieve_first_n_primes.h"   /* declares: void sieve_first_n_primes(idx_t n, uint32_t **primes_out, size_t *cnt_out); */

static uint32_t *prime_arr   = NULL;   /* array of the first primes */
static size_t    prime_cnt   = 0;      /* number of primes stored */
static seq_t     rec[20]      = {0};   /* record positions, 20 slots are enough for all known terms */
static size_t    rec_cnt      = 0;      /* current number of records */
static bool      initialized  = false;

/* One‑time initialization: generate primes and identify record indices */
static void init_records(void)
{
    if (initialized)
        return;

    /* A small upper bound: the 200th prime is 1223, which is far beyond the final record k=48. */
    const idx_t MAX_PRIMES = 200;
    sieve_first_n_primes(MAX_PRIMES, &prime_arr, &prime_cnt);   /* prime_arr now holds the first MAX_PRIMES primes */

    seq_t sum       = 0;      /* cumulative sum of primes */
    seq_t best_num  = 0;      /* numerator of best ratio seen so far */
    seq_t best_den  = 1;      /* denominator of best ratio seen so far */

    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t     k = (idx_t)i + 1;            /* 1‑based index of the prime */
        seq_t     p = (seq_t)prime_arr[i];     /* prime(k) as seq_t */

        sum += p;                             /* update cumulative sum */

        seq_t num = (seq_t)k * p;             /* k * prime(k) */
        seq_t den = sum;                      /* Σ₁ᵏ prime(j) */

        /* Compare num/den with best_num/best_den using idx_t to avoid overflow. */
        if (best_num == 0 ||
            ( (idx_t)num * (idx_t)best_den > (idx_t)best_num * (idx_t)den )) {

            if (rec_cnt < sizeof(rec)/sizeof(rec[0]))
                rec[rec_cnt++] = (seq_t)k;    /* store the record position */

            best_num  = num;
            best_den  = den;
        }
    }

    initialized = true;
    /* prime_arr is intentionally not freed; it lives for the lifetime of the process. */
}

/* Public access function required by the template. */
seq_t a(idx_t n)
{
    init_records();

    if (n == 0)                /* not defined for n<1 */
        return 0;

    idx_t idx = n - 1;
    if (idx < rec_cnt)
        return rec[idx];

    /* The sequence is finite (14 terms). Returning 0 for values beyond the end. */
    return 0;
}
