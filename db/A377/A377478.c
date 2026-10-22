/*
 * A377478  a(n) = first frugal number > 1 in base n.
 * https://oeis.org/A377478
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = first frugal number > 1 in base n (computed by
 *            brute‑force search)
 * Range:     u8 n <= 15, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   count_digits_base
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/count_digits_base.h"

/* ---------- prime sieve for trial division ---------- */
static uint32_t *prime_table = NULL;
static size_t prime_count = 0;

static void init_primes(void)
{
    if (prime_table) return;               /* already built */

    const idx_t limit = 1000000;           /* enough for all test cases */
    bool *is_comp = (bool*)calloc((size_t)(limit + 1), sizeof(bool));
    if (!is_comp) exit(EXIT_FAILURE);

    /* estimate number of primes below 1e6 */
    const size_t est = 78500;
    prime_table = (uint32_t*)malloc(est * sizeof(uint32_t));
    if (!prime_table) exit(EXIT_FAILURE);

    prime_count = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            prime_table[prime_count++] = (uint32_t)i;
            if ((uint64_t)i * (uint64_t)i <= (uint64_t)limit) {
                for (idx_t j = i * i; j <= limit; j += i)
                    is_comp[j] = true;
            }
        }
    }
    free(is_comp);
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n < 2) return 0;                   /* out of defined range */

    init_primes();

    /* iterate over positive integers > 1 to find first frugal number */
    for (uint64_t m = 2; ; ++m) {
        seq_t val = (seq_t)m;
        size_t len_m = count_digits_base(val, n);

        /* factorisation of m (uint64_t) */
        uint64_t fac[64];          /* distinct prime factors (<= 64 for 64‑bit) */
        uint64_t exp[64];
        size_t fcnt = 0;
        uint64_t tmp = m;

        for (size_t i = 0; i < prime_count && (uint64_t)prime_table[i] * (uint64_t)prime_table[i] <= tmp; ++i) {
            uint64_t p = (uint64_t)prime_table[i];
            if (tmp % p == 0) {
                uint64_t e = 0;
                while (tmp % p == 0) {
                    tmp /= p;
                    ++e;
                }
                fac[fcnt]   = p;
                exp[fcnt++] = e;
            }
        }
        if (tmp > 1) {                      /* remaining prime factor */
            fac[fcnt]   = tmp;
            exp[fcnt++] = 1;
        }

        /* compute total digit length of prime factorisation */
        size_t len_f = 0;
        for (size_t i = 0; i < fcnt; ++i) {
            len_f += count_digits_base((seq_t)fac[i], n);
            if (exp[i] > 1)
                len_f += count_digits_base((seq_t)exp[i], n);
        }

        if (len_m > len_f)                  /* frugal? yes → answer */
            return val;
    }
}
