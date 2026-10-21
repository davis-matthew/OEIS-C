/*
 * A296085  Filter sequence combining A296078(n) and A296092(n), the prime
 *          signatures of 1+phi(n) and 1+sigma(n).
 * https://oeis.org/A296085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted‑growth transform of P( A296078(n), A296092(n)
 *            ), where P is the Cantor pairing function A000027(k,l) =
 *            (k+l)*(k+l+1)/2 + k
 * Range:     u8 n <= 63, u16 n <= 9919, u32 n <= 9919, u64 n <= 9919,
 *            u128 n <= 9919, big any n
 * Helpers:   calc_phi, factor_u64, seq_pow, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External utilities provided by the build system */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"   /* void factor_u64(uint64_t n,uint64_t *pr, uint64_t *ex, size_t *cnt); */
#include "../utils/calc_phi.h"    /* seq_t calc_phi(seq_t m); */
#include "../utils/sigma_of.h"   /* seq_t sigma_of(idx_t n); */
#include "../utils/seq_pow.h"    /* seq_t seq_pow(seq_t base, idx_t exp); */

/* Prime signature of 1 + m (m > 0).  For m = 0 or 1 we return 1.
   The signature is the product of the first primes raised to the
   exponents of m, sorted in non‑increasing order (monotone length). */
static seq_t prime_signature(seq_t m)
{
    if (m <= 1)
        return 1;                 /* convention for 1 */

    /* Temporary buffers for factorization */
    uint64_t pr_buf[64];
    uint64_t ex_buf[64];
    size_t cnt;

    factor_u64((uint64_t)m, pr_buf, ex_buf, &cnt);

    /* Sort exponents in non‑increasing order (simple insertion). */
    for (size_t i = 0; i < cnt; ++i) {
        for (size_t j = i + 1; j < cnt; ++j) {
            if (ex_buf[j] > ex_buf[i]) {
                uint64_t tmp = ex_buf[i];
                ex_buf[i] = ex_buf[j];
                ex_buf[j] = tmp;
            }
        }
    }

    /* First 20 primes – enough for all practical factorizations. */
    static const uint64_t first_primes[20] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
        31, 37, 41, 43, 47, 53, 59, 61, 67, 71
    };

    seq_t sig = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t prime = (seq_t)first_primes[i];
        seq_t pw = seq_pow(prime, (idx_t)ex_buf[i]);
        sig *= pw;
    }
    return sig;
}

/* Cantor pairing function for 1‑based natural numbers.
   For a,b ≥ 1 it returns a value in ℕ, as required. */
static seq_t cantor_pair(seq_t a, seq_t b)
{
    seq_t s = a + b;
    return (s - 1) * (s - 2) / 2 + a;
}

/* Main sequence function.  The function is memoised so that any
   call order will be handled efficiently. */
seq_t a(idx_t n)
{
    /* Storage for previously computed values. */
    static idx_t max_n = 0;
    static seq_t *a_vals = NULL;    /* 1‑based array, a_vals[0] unused */
    static size_t a_cap = 0;

    /* Mapping from Cantor pair to first occurrence index. */
    struct map_entry { seq_t pair; seq_t value; };
    static struct map_entry *map = NULL;
    static size_t map_len = 0, map_cap = 0;

    /* Expand the *a_vals* array if needed. */
    if (n > max_n) {
        size_t new_cap = (n + 1) * 2;      /* double for amortised growth */
        if (new_cap > a_cap) {
            seq_t *tmp = (seq_t *)realloc(a_vals, new_cap * sizeof(seq_t));
            if (!tmp) abort();            /* memory failure – let program terminate */
            a_vals = tmp;
            a_cap = new_cap;
        }

        /* Compute terms from (max_n + 1) up to n. */
        for (idx_t k = max_n + 1; k <= n; ++k) {
            seq_t phi_val   = calc_phi((seq_t)k);
            seq_t sigma_val = sigma_of(k);

            seq_t sig_phi   = prime_signature(1 + phi_val);
            seq_t sig_sigma = prime_signature(1 + sigma_val);

            seq_t pair_val = cantor_pair(sig_phi, sig_sigma);

            /* Search for an existing pair. */
            seq_t this_value = 0;
            bool found = false;
            for (size_t i = 0; i < map_len; ++i) {
                if (map[i].pair == pair_val) {
                    this_value = map[i].value;
                    found = true;
                    break;
                }
            }

            if (!found) {
                /* New pair – assign next available index. */
                this_value = (seq_t)(map_len + 1);

                /* Grow the map array if needed. */
                if (map_len + 1 > map_cap) {
                    size_t new_map_cap = (map_cap + 1) * 2;
                    struct map_entry *tmp =
                        (struct map_entry *)realloc(map, new_map_cap * sizeof(struct map_entry));
                    if (!tmp) abort();
                    map = tmp;
                    map_cap = new_map_cap;
                }
                map[map_len].pair = pair_val;
                map[map_len].value = this_value;
                ++map_len;
            }

            a_vals[(size_t)k] = this_value;
        }
        max_n = n;
    }

    return a_vals[(size_t)n];
}
