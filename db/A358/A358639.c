/*
 * A358639  Number of partitions of n into at most 3 distinct nonprime parts.
 * https://oeis.org/A358639
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into at most 3 distinct
 *            nonprime parts
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n = 0 the empty partition counts as one. */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    static bool *is_prime = NULL;      /* is_prime[i] == true  → i is prime */
    static size_t prime_limit = 0;     /* array size of is_prime */
    static idx_t *nonprimes = NULL;   /* sorted list of nonprime numbers ≤ current limit */
    static size_t nonprime_cnt = 0;    /* number of nonprimes stored */

    if (n == 0) return 1;

    /* Grow sieve and nonprime list if needed */
    if ((size_t)(n + 1) > prime_limit) {
        size_t new_limit = (size_t)(n + 1);
        bool *prime = (bool*)calloc(new_limit, sizeof(bool));
        if (!prime) exit(1);
        /* Initially mark all ≥2 as prime */
        for (size_t i = 2; i < new_limit; ++i) prime[i] = true;
        /* Sieve */
        for (size_t i = 2; i * i < new_limit; ++i) {
            if (prime[i]) {
                for (size_t j = i * i; j < new_limit; j += i) prime[j] = false;
            }
        }
        prime[0] = prime[1] = false;

        /* Build nonprime list */
        size_t cnt = 0;
        for (size_t i = 1; i < new_limit; ++i) {
            if (!prime[i]) ++cnt;
        }
        idx_t *np = (idx_t*)malloc(cnt * sizeof(idx_t));
        if (!np) { free(prime); exit(1); }
        size_t idx = 0;
        for (size_t i = 1; i < new_limit; ++i) {
            if (!prime[i]) np[idx++] = i;
        }

        free(is_prime);
        free(nonprimes);
        is_prime = prime;
        nonprimes = np;
        prime_limit = new_limit;
        nonprime_cnt = cnt;
    }

    seq_t result = 0;

    /* One-part partitions (n itself must be nonprime) */
    if (!is_prime[(size_t)n]) result += 1;

    /* Two-part partitions with distinct nonprime parts */
    for (size_t ia = 0; ia < nonprime_cnt; ++ia) {
        idx_t a_val = nonprimes[ia];
        if (a_val >= n) break;
        for (size_t ib = ia + 1; ib < nonprime_cnt; ++ib) {
            idx_t b_val = nonprimes[ib];
            idx_t sum = a_val + b_val;
            if (sum > n) break;
            if (sum == n) result += 1;
        }
    }

    /* Three-part partitions with distinct nonprime parts */
    for (size_t ia = 0; ia < nonprime_cnt; ++ia) {
        idx_t a_val = nonprimes[ia];
        if (a_val >= n) break;
        for (size_t ib = ia + 1; ib < nonprime_cnt; ++ib) {
            idx_t b_val = nonprimes[ib];
            idx_t sum_ab = a_val + b_val;
            if (sum_ab > n) break;
            idx_t c_val = n - sum_ab;
            if (c_val <= b_val) continue;              /* ensure distinctness and order */
            if (!is_prime[(size_t)c_val]) result += 1;
        }
    }

    return result;
}
