/*
 * A319074  a(n) is the sum of the first n nonnegative powers of the n-th
 *          prime.
 * https://oeis.org/A319074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n-1} p_n^k = (p_n^n - 1)/(p_n - 1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 18 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Where p_n is the n‑th prime (n ≥ 1). */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

static void sieve_up_to(idx_t limit, uint64_t **primes_out, size_t *cnt_out)
{
    /* allocate bitmap of size limit+1, 0 = prime, 1 = composite */
    size_t sz = (size_t)(limit + 1);
    uint8_t *is_composite = (uint8_t *)calloc(sz, 1);
    if (!is_composite) {
        *primes_out = NULL;
        *cnt_out    = 0;
        return;
    }

    is_composite[0] = is_composite[1] = 1;

    /* simple Eratosthenes */
    for (idx_t i = 2; (i * i) <= limit; ++i) {
        if (!is_composite[i]) {
            for (idx_t j = i * i; j <= limit; j += i) {
                is_composite[j] = 1;
            }
        }
    }

    /* count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_composite[i]) ++cnt;
    }

    uint64_t *primes = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!primes) {
        free(is_composite);
        *primes_out = NULL;
        *cnt_out    = 0;
        return;
    }

    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_composite[i]) primes[pos++] = (uint64_t)i;
    }

    free(is_composite);
    *primes_out = primes;
    *cnt_out    = cnt;
}

/* ---------- prime storage ---------- */

static uint64_t *prime_array = NULL;   /* 0‑based array of primes */
static size_t   prime_count  = 0;      /* number of primes stored */

/* ---------- helper: sieve up to given limit ---------- */

/* ---------- helper: ensure we have at least n primes ---------- */

static void ensure_prime_count(idx_t n)
{
    if ((size_t)n <= prime_count) return;

    idx_t estimate = (n < 6) ? 15 : n * 20;   /* rough over‑estimate */

    while (1) {
        uint64_t *new_primes = NULL;
        size_t    new_cnt    = 0;
        sieve_up_to(estimate, &new_primes, &new_cnt);

        if (new_cnt >= (size_t)n) {
            free(prime_array);
            prime_array = new_primes;
            prime_count = new_cnt;
            return;
        }

        free(new_primes);
        estimate *= 2;   /* increase bound and try again */
    }
}

/* ---------- helper: integer power (binary exponentiation) ---------- */

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    seq_t b = base;
    while (exp > 0) {
        if (exp & 1ULL) result = result * b;
        b = b * b;
        exp >>= 1;
    }
    return result;
}

/* ---------- main sequence function ---------- */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety guard; sequence defined for n ≥ 1 */

    ensure_prime_count(n);

    /* primes array is 0‑based, n‑th prime is at index n-1 */
    seq_t p = (seq_t)prime_array[n - 1];

    seq_t p_pow_n = pow_seq(p, n);
    seq_t numerator   = p_pow_n - (seq_t)1;
    seq_t denominator = p - (seq_t)1;
    seq_t result      = numerator / denominator;

    return result;
}
