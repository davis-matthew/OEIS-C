/*
 * A391929  Number of fixed points on row n of triangle A391930.
 * https://oeis.org/A391929
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 2, for n > 1, let P_{n-1} = product of the
 *            first (n‑1) primes (A002110), let p_n = prime(n) (A000040). Then
 *            if (P_{n-1} - 1) is a multiple of p_n -> a(n) = 2·p_n else a(n)
 *            = a(n‑1) (ic. a(n) = a(n-1) + 2·p_n if (P_{n-1} ≡ 1 (mod p_n),
 *            else 0)
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* ---------- prime list (0‑indexed) ---------- */
static uint64_t *prime_arr = NULL;   /* prime_arr[i] = prime(i+1) */
static size_t   prime_cnt = 0;       /* number of primes stored */

/* Extend prime_arr until it contains at least 'needed' primes. */
static void ensure_primes_count(size_t needed)
{
    if (prime_cnt >= needed)
        return;

    if (prime_arr == NULL)
        prime_arr = (uint64_t *)malloc(sizeof(uint64_t));
    else
        prime_arr = (uint64_t *)realloc(prime_arr, needed * sizeof(uint64_t));

    if (prime_arr == NULL)
        abort();

    while (prime_cnt < needed) {
        uint64_t candidate = (prime_cnt == 0) ? 2 : prime_arr[prime_cnt - 1] + 1;
        for (;;) {
            bool is_prime = true;
            for (size_t j = 0; j < prime_cnt; ++j) {
                uint64_t p = prime_arr[j];
                if (p * p > candidate)
                    break;
                if (candidate % p == 0) {
                    is_prime = false;
                    break;
                }
            }
            if (is_prime)
                break;
            ++candidate;
        }
        prime_arr[prime_cnt++] = candidate;
    }
}

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)2;

    /* we need prime(n) and the first (n‑1) primes */
    ensure_primes_count((size_t)n);

    seq_t current = (seq_t)2;   /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        uint64_t p_i = prime_arr[i - 1];            /* prime(i)   */
        uint64_t prod_mod = 1;                      /* product of primes[0..i-2] modulo p_i */

        for (size_t j = 0; j < (size_t)(i - 1); ++j) {
            prod_mod =
                (uint64_t)(((__uint128_t)prod_mod * prime_arr[j]) % p_i);
        }

        if (prod_mod == 1)                           /* (P_{i-1} ≡ 1 (mod p_i)) */
            current = (seq_t)((seq_t)2 * (seq_t)p_i);
        /* else current stays the same (a(i-1)) */
    }

    return current;
}
