/*
 * A260405  Irregular array read by rows, where the n-th row lists the primes
 *          p < A002110(n) such that 2*A002110(n) - p is also prime.
 * https://oeis.org/A260405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) enumerates primes p < P(n) (the primorial of the first n
 *            primes) * such that 2·P(n) – p is also prime. The terms are
 *            emitted in the order * they appear when rows n = 1, 2, … are
 *            processed. The sequence works * for arbitrary call order through
 *            a cache
 * Range:     u8 n <= 7, u16 n <= 420, u32 n <= 1056, u64 n <= 9841,
 *            u128 n <= 9841, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* --------------------------------------------------------------------- */
/*  Includes – required for both C and C++ builds                         */
/* --------------------------------------------------------------------- */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

/* --------------------------------------------------------------------- */
/*  Sieve parameters and data                                            */
/* --------------------------------------------------------------------- */
#define SIEVE_LIMIT 20000000 /* 20 million – more than enough for n≈10 000 */

static uint8_t *sieve_bits = NULL;   /* 0 = prime, 1 = composite */
static uint32_t *prime_list = NULL;  /* list of primes up to SIEVE_LIMIT */
static size_t   prime_cnt   = 0;

/* Build the sieve only once */
static void build_sieve(void)
{
    if (sieve_bits != NULL) return;          /* already built */

    sieve_bits = (uint8_t*)malloc(SIEVE_LIMIT + 1);
    if (!sieve_bits) return;                 /* allocation failure – should not happen */

    memset(sieve_bits, 0, SIEVE_LIMIT + 1);
    sieve_bits[0] = sieve_bits[1] = 1;

    for (uint32_t i = 2; i * i <= SIEVE_LIMIT; ++i)
        if (!sieve_bits[i])
            for (uint32_t j = i * i; j <= SIEVE_LIMIT; j += i)
                sieve_bits[j] = 1;

    /* count primes first */
    for (uint32_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (!sieve_bits[i]) prime_cnt++;

    prime_list = (uint32_t*)malloc(prime_cnt * sizeof(uint32_t));
    size_t idx = 0;
    for (uint32_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (!sieve_bits[i]) prime_list[idx++] = i;
}

/* --------------------------------------------------------------------- */
/*  Sequence cache                                                        */
/* --------------------------------------------------------------------- */
static seq_t  *seq_arr = NULL;   /* cached sequence values          */
static size_t  seq_len = 0;      /* number of stored terms          */
static size_t  seq_cap = 0;      /* capacity of seq_arr             */

static idx_t   max_requested = 0;    /* largest n requested so far     */
static idx_t   current_row   = 0;    /* rows processed so far          */

/* Product of the first current_row primes – use 64‑bit to keep the
 * product from overflowing during the row generation.  The variable
 * name is changed to avoid conflict with GMP's primorial function. */
static uint64_t primorial_val = 1;

/* Append one element to the sequence cache */
static void append_value(seq_t val)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 8;
        seq_t *new_arr = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (seq_arr) {
            memcpy(new_arr, seq_arr, seq_len * sizeof(seq_t));
            free(seq_arr);
        }
        seq_arr = new_arr;
        seq_cap = new_cap;
    }
    seq_arr[seq_len++] = val;
}

/* --------------------------------------------------------------------- */
/*  Main sequence function                                               */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;              /* undefined for n=0, but should never be called */

    if (n > max_requested) {
        max_requested = n;

        /* Ensure the sieve is ready */
        build_sieve();

        /* Generate rows until we have at least max_requested terms */
        while (seq_len < (size_t)max_requested) {
            current_row++;                    /* next row number (1‑based) */

            /* Multiply primorial_val by the current prime to get P(n) */
            uint32_t nth_prime = prime_list[current_row - 1];
            primorial_val *= (uint64_t)nth_prime;   /* use 64‑bit to keep product small */

            uint64_t P = primorial_val;            /* primorial for this row */

            /* Enumerate primes p < P and test the symmetric condition */
            for (size_t i = 0; i < prime_cnt && prime_list[i] < (uint32_t)P; ++i) {
                uint32_t p = prime_list[i];
                uint64_t cand = 2 * P - (uint64_t)p;   /* 2*P – p */
                if (is_prime_big((seq_t)cand))
                    append_value((seq_t)p);
            }
        }
    }

    /* Return the requested term (1‑based indexing) */
    return seq_arr[n - 1];
}
