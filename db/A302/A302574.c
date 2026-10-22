/*
 * A302574  Primitive unitary abundant numbers (definition 2): unitary
 *          abundant numbers (A034683) having no unitary abundant proper
 *          unitary divisor.
 * https://oeis.org/A302574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 1107, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth primitive unitary abundant number (Definition 2 of A302574). */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static state for memoisation */
    static seq_t *terms = NULL;     /* array of found primitive unitary abundant numbers */
    static size_t terms_len = 0;    /* number of terms stored */
    static size_t terms_cap = 0;    /* capacity of allocated array */
    static idx_t next_candidate = 1;/* last integer that has been tested */

    /* Generate terms until we have at least n of them */
    while (terms_len < n) {
        ++next_candidate;

        /* Factorisation of next_candidate */
        uint64_t temp = (uint64_t)next_candidate;
        seq_t prime_pow[16];  /* prime powers p^a of the factorisation */
        size_t k = 0;         /* number of distinct prime factors */

        /* Handle factor 2 */
        if ((temp & 1U) == 0) {
            seq_t pow = 1;
            while ((temp & 1U) == 0) {
                temp >>= 1;
                pow <<= 1;
            }
            prime_pow[k++] = pow;
        }

        /* Handle odd factors */
        for (uint64_t p = 3; p * p <= temp; p += 2) {
            if (temp % p == 0) {
                seq_t pow = 1;
                while (temp % p == 0) {
                    temp /= p;
                    pow *= (seq_t)p;
                }
                prime_pow[k++] = pow;
            }
        }

        /* Remaining prime factor, if any */
        if (temp > 1) {
            prime_pow[k++] = (seq_t)temp;
        }

        /* Compute uσ(n) = Π (1 + p^a) for all prime powers */
        seq_t sigma_total = 1;
        for (size_t i = 0; i < k; ++i) {
            sigma_total *= (prime_pow[i] + 1);
        }

        /* Check if n is unitary abundant */
        seq_t rhs = (seq_t)2 * (seq_t)next_candidate;
        if (sigma_total <= rhs) {
            continue; /* not unitary abundant */
        }

        /* Check for proper unitary abundant divisors */
        bool is_primitive = true;
        if (k > 0) {
            size_t total_masks = ((size_t)1 << k);
            for (size_t mask = 1; mask < total_masks - 1 && is_primitive; ++mask) {
                seq_t d_val = 1;
                seq_t sigma_d = 1;
                for (size_t j = 0; j < k; ++j) {
                    if (mask & ((size_t)1 << j)) {
                        d_val *= prime_pow[j];
                        sigma_d *= (prime_pow[j] + 1);
                    }
                }
                seq_t rhs_d = (seq_t)2 * d_val;
                if (sigma_d > rhs_d) {
                    /* d is a unitary abundant proper divisor of n */
                    is_primitive = false;
                    break;
                }
            }
        }

        /* Store primitive unitary abundant number */
        if (is_primitive) {
            if (terms_len == terms_cap) {
                terms_cap = (terms_cap ? terms_cap * 2 : 8);
                seq_t *tmp = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
                terms = tmp;
            }
            terms[terms_len++] = (seq_t)next_candidate;
        }
    }

    return terms[n - 1];
}
