/*
 * A373437  Integers k such that sigma(sigma(2*k))=2*sigma(sigma(k));
 *          sigma=A000203.
 * https://oeis.org/A373437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Integers k such that σ(σ(2*k)) = 2*σ(σ(k)),
   where σ = sum_of_divisors.  The function returns the nth element
   of this sequence (n>=1).  It generates terms lazily and stores
   them in a static array so that arbitrary call order is supported.  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

#ifdef __cplusplus
#include <cstddef>
#include <cstdint>
#else
/* For C: the standard headers above are sufficient */
#endif

/* -------------------------------------------------------------------- */
/* Prime sieve – generates all primes <= LIMIT into a static array.    */
/* -------------------------------------------------------------------- */
static uint64_t *prime_list = NULL;
static size_t prime_count  = 0;
static const uint64_t PRIME_LIMIT = 2000000ULL; /* >= sqrt of max σ(σ(k)) needed */

static void build_primes(void)
{
    if (prime_list != NULL) return;

    /* Simple bitset sieve */
    /* Use dynamic allocation for the sieve bitmap */
    uint8_t *is_composite = (uint8_t *)calloc((size_t)PRIME_LIMIT + 1, 1);
    if (!is_composite) exit(1);

    for (uint64_t i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (!is_composite[i]) {
            for (uint64_t j = i * i; j <= PRIME_LIMIT; j += i)
                is_composite[j] = 1;
        }
    }

    /* Count primes */
    size_t cnt = 0;
    for (uint64_t i = 2; i <= PRIME_LIMIT; ++i)
        if (!is_composite[i]) ++cnt;

    /* Allocate and store primes */
    prime_list = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!prime_list) exit(1);
    size_t idx = 0;
    for (uint64_t i = 2; i <= PRIME_LIMIT; ++i)
        if (!is_composite[i]) prime_list[idx++] = i;

    prime_count = cnt;
    free(is_composite);
}

/* -------------------------------------------------------------------- */
/* σ(n) – sum of (positive) divisors of n.  The argument n fits in u64.*/
static seq_t sigma_u64(uint64_t n)
{
    build_primes(); /* ensure primes ready */

    seq_t result = 1;
    uint64_t m = n;

    for (size_t i = 0; i < prime_count && prime_list[i] * prime_list[i] <= m; ++i) {
        uint64_t p = prime_list[i];
        if (m % p == 0) {
            seq_t term = 1;          /* p⁰ */
            uint64_t power = 1;      /* current power of p */
            while (m % p == 0) {
                m /= p;
                power *= p;
                term += (seq_t)power;
            }
            result *= term;
        }
    }
    if (m > 1) {
        /* m is a prime factor > sqrt(n) */
        result *= ((seq_t)1 + (seq_t)m);
    }
    return result;
}

/* -------------------------------------------------------------------- */
/* Static storage for generated sequence and auxiliary data            */
/* -------------------------------------------------------------------- */
static seq_t *seq_array  = NULL;   /* stored terms */
static size_t seq_len    = 0;      /* current number of stored terms */
static size_t seq_cap    = 0;      /* allocated capacity */
static uint64_t next_k   = 1;      /* next k to test */

/* -------------------------------------------------------------------- */
/* Utility: grow the sequence array if needed                          */
/* -------------------------------------------------------------------- */
static void ensure_sequence_capacity(size_t required)
{
    if (seq_cap >= required) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 128;
    while (new_cap < required) new_cap <<= 1;

#ifdef __cplusplus
    seq_t *tmp = new seq_t[new_cap];
    for (size_t i = 0; i < seq_len; ++i) tmp[i] = seq_array[i];
    delete[] seq_array;
    seq_array = tmp;
#else
    seq_t *tmp = (seq_t *)realloc(seq_array, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    seq_array = tmp;
#endif
    seq_cap = new_cap;
}

/* -------------------------------------------------------------------- */
/* Main function – returns the nth term of the sequence                 */
/* -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n=0, but harmless */

    /* Ensure we have enough terms */
    while (seq_len < (size_t)n) {
        uint64_t k = next_k++;

        /* Compute σ(k), σ(2k)  (all fit in uint64_t for our n range) */
        seq_t sig_k   = sigma_u64(k);
        seq_t sig_2k  = sigma_u64(k << 1);   /* 2*k, safe as k is small */

        /* If σ(k) or σ(2k) exceeds 64-bit, skip this k (unlikely) */
        if (sig_k > (seq_t)UINT64_MAX || sig_2k > (seq_t)UINT64_MAX)
            continue;

        uint64_t sig_k_u64  = (uint64_t)sig_k;
        uint64_t sig_2k_u64 = (uint64_t)sig_2k;

        /* Compute σ(σ(k)) and σ(σ(2k)) */
        seq_t sig_sig_k  = sigma_u64(sig_k_u64);
        seq_t sig_sig_2k = sigma_u64(sig_2k_u64);

        /* Check the defining condition */
        if (sig_sig_2k == ((seq_t)2) * sig_sig_k) {
            /* Store k in the sequence */
            ensure_sequence_capacity(seq_len + 1);
            seq_array[seq_len++] = k;
        }
    }

    return seq_array[n - 1];
}
