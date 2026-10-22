/*
 * A325352  Heinz number of the differences plus one of the integer partition
 *          with Heinz number n.
 * https://oeis.org/A325352
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of primes(differ+1) where differ are successive *
 *            differences of the partition of n into Heinz primes (indices of
 *            its prime * factors). The code factors n, reorders the
 *            prime‑indices descending, * forms the differences+1 sequence and
 *            returns its Heinz number
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* Upper limit for precomputation.  Handles all n up to about 200 000. */
#define LIMIT 200000

static uint32_t primes[LIMIT + 1];       /* 0‑based array of primes */
static size_t   primes_cnt = 0;          /* number of primes found */
static uint32_t prime_index[LIMIT + 1];   /* prime_index[p] = index of p (0‑based) */
static uint32_t spf[LIMIT + 1];          /* smallest prime factor for each n ≤ LIMIT */
static bool     primes_initialized = false;

/* One‑time sieve initialization */
static void init_primes(void)
{
    if (primes_initialized) return;

    for (uint32_t i = 0; i <= LIMIT; ++i) {
        spf[i] = i;
        prime_index[i] = 0xFFFFFFFF;    /* invalid */
    }

    /* SPF sieve */
    for (uint32_t i = 2; (uint64_t)i * i <= LIMIT; ++i) {
        if (spf[i] == i) {
            for (uint32_t j = i * i; j <= LIMIT; j += i) {
                if (spf[j] == j) spf[j] = i;
            }
        }
    }

    /* Store primes and their indices */
    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (spf[i] == i) {              /* i is prime */
            primes[primes_cnt++] = i;
            prime_index[i] = (uint32_t)(primes_cnt - 1);  /* 0‑based */
        }
    }

    primes_initialized = true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    init_primes();

    if (n == 1) return (seq_t)1;    /* special case */

    /* Factor n using precomputed SPF array */
    uint32_t temp = (uint32_t)n;
    uint32_t parts[64];    /* indices of prime factors (1‑based), multiplicities kept */
    size_t   parts_cnt = 0;

    while (temp > 1) {
        uint32_t p = spf[temp];
        uint32_t idx = prime_index[p] + 1;   /* convert to 1‑based index of prime p */
        parts[parts_cnt++] = idx;
        temp /= p;
    }

    /* Sort indices in descending order (simple insertion sort, size is tiny) */
    for (size_t i = 0; i < parts_cnt; ++i) {
        for (size_t j = i + 1; j < parts_cnt; ++j) {
            if (parts[j] > parts[i]) {
                uint32_t t = parts[i];
                parts[i] = parts[j];
                parts[j] = t;
            }
        }
    }

    /* Compute differences + 1 */
    uint32_t diffp[64];
    size_t   diff_cnt = 0;
    for (size_t i = 0; i + 1 < parts_cnt; ++i) {
        uint32_t d = parts[i] - parts[i + 1];
        diffp[diff_cnt++] = d + 1;
    }

    /* Compute Heinz number of the new partition */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < diff_cnt; ++i) {
        uint32_t h = diffp[i];          /* 1‑based index of prime */
        result *= (seq_t)primes[h - 1];
    }

    return result;
}
