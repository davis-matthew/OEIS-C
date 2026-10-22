/*
 * A343000  Discriminants of cyclic cubic fields.
 * https://oeis.org/A343000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (k(n))^2, where k(n) is the n-th smallest integer of the
 *            form * k = (product of distinct primes p ≡ 1 (mod 3)) or k = 9 *
 *            (product of distinct primes p ≡ 1 (mod 3)). * All such k are
 *            sorted increasingly and their squares form the OEIS sequence
 *            A343000
 * Range:     u8 n <= 3, u16 n <= 33, u32 n <= 3200, u64 n <= 3200,
 *            u128 n <= 3200, big any n
 * Helpers:   generate_products_rec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/generate_products_rec.h"

/* Helper to compare uint64_t values for qsort. */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t aa = *(const uint64_t *)a;
    uint64_t bb = *(const uint64_t *)b;
    return (aa < bb) ? -1 : (aa > bb) ? 1 : 0;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* undefined for 0, return 0 as safe default */

    /* Initial bound for k.  It is large enough to cover the first 10 000 terms
       of the sequence.  The value can be increased automatically if needed. */
    uint64_t limit = 2000000ULL;

    while (1) {
        /* ------------------------------------------------------------------
           1. Generate all primes up to 'limit'.
           ------------------------------------------------------------------ */
        bool *is_composite = (bool *)calloc((size_t)limit + 1U, sizeof(bool));
        if (!is_composite) return (seq_t)0;  /* allocation failure */

        for (uint64_t i = 2; i * i <= limit; ++i) {
            if (!is_composite[i]) {
                for (uint64_t j = i * i; j <= limit; j += i)
                    is_composite[j] = true;
            }
        }

        /* Collect primes p ≡ 1 (mod 3). */
        size_t primes_cap = 128;
        size_t primes_len = 0;
        uint64_t *primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
        if (!primes) { free(is_composite); return (seq_t)0; }

        for (uint64_t i = 2; i <= limit; ++i) {
            if (!is_composite[i] && (i % 3ULL) == 1ULL) {
                if (primes_len == primes_cap) {
                    primes_cap <<= 1;
                    primes = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
                }
                primes[primes_len++] = i;
            }
        }
        free(is_composite);

        /* ------------------------------------------------------------------
           2. Generate all square‑free products of the collected primes,
              each product <= limit.
           ------------------------------------------------------------------ */
        size_t prod_cap = 128;
        size_t prod_len = 0;
        uint64_t *products = (uint64_t *)malloc(prod_cap * sizeof(uint64_t));
        if (!products) { free(primes); return (seq_t)0; }

        generate_products_rec(0, 1ULL, primes, primes_len, limit,
                              &products, &prod_len, &prod_cap);

        /* ------------------------------------------------------------------
           3. Build the list of k values:
              - all products (prime products),
              - the number 9,
              - 9 multiplied by each product that keeps the value <= limit.
           ------------------------------------------------------------------ */
        size_t k_cap = prod_len * 2 + 128;
        size_t k_len = 0;
        uint64_t *k_values = (uint64_t *)malloc(k_cap * sizeof(uint64_t));
        if (!k_values) {
            free(primes); free(products); return (seq_t)0;
        }

        /* Add the products themselves. */
        for (size_t i = 0; i < prod_len; ++i) {
            if (k_len == k_cap) {
                k_cap <<= 1;
                k_values = (uint64_t *)realloc(k_values, k_cap * sizeof(uint64_t));
            }
            k_values[k_len++] = products[i];
        }

        /* Add the stand‑alone 9. */
        if (k_len == k_cap) {
            k_cap <<= 1;
            k_values = (uint64_t *)realloc(k_values, k_cap * sizeof(uint64_t));
        }
        k_values[k_len++] = 9ULL;

        /* Add 9 * product where result stays within the limit. */
        for (size_t i = 0; i < prod_len; ++i) {
            if (products[i] <= limit / 9ULL) {
                uint64_t val = products[i] * 9ULL;
                if (k_len == k_cap) {
                    k_cap <<= 1;
                    k_values = (uint64_t *)realloc(k_values, k_cap * sizeof(uint64_t));
                }
                k_values[k_len++] = val;
            }
        }

        free(primes);
        free(products);

        /* ------------------------------------------------------------------
           4. Sort k_values and remove duplicates (unlikely but harmless).
           ------------------------------------------------------------------ */
        qsort(k_values, k_len, sizeof(uint64_t), cmp_uint64);

        size_t uniq_k_cap = k_len;
        uint64_t *uniq_k = (uint64_t *)malloc(uniq_k_cap * sizeof(uint64_t));
        if (!uniq_k) { free(k_values); return (seq_t)0; }

        size_t uniq_len = 0;
        for (size_t i = 0; i < k_len; ++i) {
            if (uniq_len == 0 || uniq_k[uniq_len - 1] != k_values[i]) {
                uniq_k[uniq_len++] = k_values[i];
            }
        }
        free(k_values);

        /* ------------------------------------------------------------------
           5. If we have enough terms, compute and return the requested one.
           ------------------------------------------------------------------ */
        if (n <= (idx_t)uniq_len) {
            uint64_t k = uniq_k[n - 1];
            seq_t k_seq = (seq_t)k;
            seq_t result = k_seq * k_seq;      /* k^2 in seq_t type */
            free(uniq_k);
            return result;
        }

        /* ------------------------------------------------------------------
           6. Not enough terms: enlarge the bound and try again.
           ------------------------------------------------------------------ */
        free(uniq_k);
        limit <<= 1;   /* double the limit */
    } /* loop */
}
