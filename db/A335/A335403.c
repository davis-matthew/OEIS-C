/*
 * A335403  If n = Product_{i=1..k} p_i^e_i then a(n) = Sum_{i=1..k} e_i *
 *          prime(p_i).
 * https://oeis.org/A335403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, changed
 * Formula:   a(n) = Sum_{p^e||n} e * prime(p)
 * Range:     u8 n <= 58, u16 n <= 6546, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* prime(p) = the p‑th prime number.           */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/sieve_first_n_primes.h"

/* Global storage for the list of primes (1‑based indexing: prime_list[0] = 2) */
static uint32_t *prime_list = NULL;   /* array of primes */
static size_t   prime_cnt  = 0;       /* number of primes stored */

/* Ensure that prime_list contains at least 'required' primes. */
static void ensure_primes_count(idx_t required)
{
    if (required == 0 || prime_cnt >= required) return;

    uint32_t *tmp = NULL;
    size_t   tmp_cnt = 0;
    sieve_first_n_primes(required, (uint32_t **)&tmp, &tmp_cnt);

    /* Replace the old array, freeing memory if necessary. */
    free(prime_list);
    prime_list = tmp;
    prime_cnt  = tmp_cnt;
}

/* Public function that computes a(n). */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined below offset, return 0 */

    if (n == 1) return 0;         /* a(1) = 0 by definition */

    /* Factorise n. */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t   cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    seq_t result = 0;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        uint64_t e = ex[i];

        /* Ensure we have enough primes for index 'p'. */
        ensure_primes_count((idx_t)p);

        /* prime(p) is stored at prime_list[p-1] (0‑based). */
        seq_t prime_val = (seq_t)prime_list[(size_t)(p - 1)];

        result += (seq_t)e * prime_val;
    }
    return result;
}
