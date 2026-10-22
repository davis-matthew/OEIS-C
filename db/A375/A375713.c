/*
 * A375713  Indices of consecutive non-prime-powers (A361102) differing by 1.
 *          Numbers k such that the k-th and (k+1)-th non-prime-powers differ
 *          by just one.
 * https://oeis.org/A375713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A375713: indices k such that the k‑th and
   (k+1)-th non‑prime‑powers differ by 1. The implementation performs an
   incremental scan of all integers, tests for non‑prime‑powers, and
   records the index when two consecutive non‑prime‑powers differ by 1. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* External types supplied by the build system */
extern seq_t a(idx_t n);

/* --- dynamic array to store found indices -------------------------------- */
static void push_idx(seq_t **arr, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
    (*arr)[(*len)++] = val;
}

/* --- prime list for factorisation --------------------------------------- */
static uint64_t *primes = NULL;
static size_t primes_cnt = 0;
static size_t primes_cap = 0;
static uint64_t prime_limit = 0;

/* Simple sieve to populate primes up to 'limit' */
static void init_primes(uint64_t limit)
{
    uint64_t i, j;
    uint8_t *is_comp = (uint8_t *)calloc(limit + 1, 1);
    if (!is_comp) exit(EXIT_FAILURE);

    primes_cap = (limit / 10) + 10;
    primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
    if (!primes) exit(EXIT_FAILURE);
    primes_cnt = 0;

    for (i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            if (primes_cnt == primes_cap) {
                primes_cap <<= 1;
                uint64_t *tmp = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
                if (!tmp) exit(EXIT_FAILURE);
                primes = tmp;
            }
            primes[primes_cnt++] = i;

            if (i * i <= limit) {
                for (j = i * i; j <= limit; j += i)
                    is_comp[j] = 1;
            }
        }
    }
    free(is_comp);
    prime_limit = limit;
}

/* Ensure that the prime list covers sqrt(n) */
static void ensure_primes(idx_t n)
{
    uint64_t need = (uint64_t)(sqrt((double)n) + 1.0);
    if (need <= prime_limit) return;
    init_primes(need);
}

/* --- check if a number is a prime power --------------------------------- */
static bool is_prime_power(idx_t n)
{
    if (n <= 1) return false;  /* 1 is not a prime power */

    ensure_primes(n);
    idx_t temp = n;

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > temp) break;
        if (temp % p == 0) {
            while (temp % p == 0) temp /= p;
            return temp == 1;  /* true iff all prime factors are the same */
        }
    }
    return true;  /* n itself is prime (prime power of exponent 1) */
}

/* --- state for incremental scanning ------------------------------------- */
static seq_t *indices   = NULL;   /* array of indices k satisfying condition */
static size_t idx_len = 0;        /* number of indices stored */
static size_t idx_cap = 0;        /* capacity of indices array */

static idx_t last_checked        = 0;   /* last integer processed */
static idx_t last_nonprime_index = 0;   /* index of last non‑prime‑power */
static idx_t last_nonprime       = 0;   /* value of last non‑prime‑power */

/* --- main sequence accessor ---------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for OEIS offset, defensive */

    /* compute until we have at least n indices */
    while (idx_len < n) {
        ++last_checked;

        if (!is_prime_power(last_checked)) {
            ++last_nonprime_index;          /* 1‑based index of this non‑prime‑power */

            if (last_nonprime != 0 &&
                last_checked - last_nonprime == 1) {
                /* store index of the preceding non‑prime‑power */
                push_idx(&indices, &idx_len, &idx_cap,
                         (seq_t)(last_nonprime_index - 1));
            }
            last_nonprime = last_checked;
        }
    }
    return indices[n - 1];
}
