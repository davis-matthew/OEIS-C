/*
 * A354859  a(n) is the numerator of 1/prime(n) + 2/prime(n-1) + 3/prime(n-2)
 *          + ... + (n-1)/3 + n/2.
 * https://oeis.org/A354859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of ∑_{i=1..n} (n-i+1)/prime(i)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 20,
 *            big any n
 * Helpers:   _prime_limit_for_idx, seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: compute the common denominator L = ∏_{k=1..n} prime(k),
   then numerator = ∑_{i=1..n} (n-i+1) * (L / prime(i)),
   finally reduce by the GCD of numerator and L. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/seq_gcd.h"

/* dynamically grow a list of primes */
static uint32_t *prime_list = NULL;
static size_t   prime_cap   = 0;
static size_t   prime_cnt   = 0;

/* Build or grow the list of the first 'need' primes. */
static void ensure_primes(idx_t need)
{
    if (prime_cnt >= (size_t)need)
        return;                               /* already have enough */

    /* approximate upper bound for the n-th prime (safe). */
    size_t limit = _prime_limit_for_idx(need);
    size_t sz = limit + 1;

    /* allocate temporary sieve array */
    bool *is_comp = (bool *)calloc(sz, 1);
    size_t cnt = 0;

    for (size_t p = 2; p < sz; ++p) {
        if (!is_comp[p]) {
            if (cnt == prime_cap) {
                prime_cap = prime_cap ? prime_cap * 2 : 8192;
                prime_list = (uint32_t *)realloc(prime_list,
                                                 prime_cap * sizeof(uint32_t));
            }
            prime_list[cnt++] = (uint32_t)p;
            /* cross‑out multiples – careful about overflow */
            for (unsigned long long j = (unsigned long long)p * p;
                 j < sz; j += p)
                is_comp[(size_t)j] = true;
        }
    }
    free(is_comp);

    prime_cnt = cnt;
}

seq_t a(idx_t n)
{
    /* n >= 1 by sequence offset */
    ensure_primes(n);

    /* compute product L = ∏_{k=1..n} prime(k) */
    seq_t prod = (seq_t)1;
    for (size_t i = 0; i < (size_t)n; ++i)
        prod *= (seq_t)prime_list[i];

    /* compute sum numerator: Σ (n-i) * (L / prime_i) with 0‑based i */
    seq_t sum = (seq_t)0;
    for (size_t i = 0; i < (size_t)n; ++i) {
        seq_t term  = prod / (seq_t)prime_list[i];
        seq_t coeff = (seq_t)(n - i);      /* 1‑based n-i+1 */
        sum += term * coeff;
    }

    /* reduce the fraction by GCD of numerator and denominator (prod). */
    seq_t g = seq_gcd(sum, prod);
    return sum / g;
}
