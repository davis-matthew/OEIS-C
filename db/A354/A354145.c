/*
 * A354145  Deficient numbers that are neither prime powers nor semiprimes.
 * https://oeis.org/A354145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 6874, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A354145: deficient numbers that are neither prime powers nor semiprimes. Search method. */
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* Static storage for computed terms and state. */
    static seq_t *terms = NULL;      /* array of sequence values */
    static size_t terms_len = 0;     /* number of terms stored */
    static size_t terms_cap = 0;     /* capacity of the array */
    static idx_t next_check = 1;     /* next integer to test */

    /* Grow the terms array when needed. */
    if (terms_len == terms_cap) {
        terms_cap = terms_cap ? terms_cap * 2 : 16;
        terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least 'n' of them. */
    while ((size_t)n > terms_len) {
        /* Move to the next candidate integer. */
        ++next_check;

        idx_t temp = next_check;
        idx_t omega = 0;        /* total number of prime factors (with multiplicity) */
        idx_t distinct = 0;     /* number of distinct prime factors */
        seq_t sigma = 1;        /* sum of divisors (including the number itself) */

        /* Factorization using trial division. */
        for (idx_t p = 2; p <= temp / p; ++p) {
            if (temp % p == 0) {
                ++distinct;
                idx_t exp = 0;
                seq_t pow = 1;
                seq_t sum_exp = 1;    /* 1 + p + p^2 + ... + p^exp */

                while (temp % p == 0) {
                    temp /= p;
                    ++exp;
                    ++omega;
                    pow *= (seq_t)p;
                    sum_exp += pow;
                }
                sigma *= sum_exp;
            }
        }
        /* Remaining prime factor (if any). */
        if (temp > 1) {
            ++distinct;
            ++omega;
            sigma *= ((seq_t)temp + 1);
        }

        /* Check the conditions: deficient, not a prime power, not a semiprime. */
        bool is_deficient = sigma < ((seq_t)next_check * 2);
        if (omega > 2 && distinct > 1 && is_deficient) {
            /* Store the term. */
            if (terms_len == terms_cap) {
                terms_cap = terms_cap ? terms_cap * 2 : 16;
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_len++] = (seq_t)next_check;
        }
    }

    /* Return the requested term (1‑based index). */
    return terms[(size_t)(n - 1)];
}
