/*
 * A289146  Numbers with decimal expansion d_1,d_2,...,d_k that are divisible
 *          by sigma_0(d_k) + sigma_1(d_k-1) + ... + sigma_k-1(d_1).
 * https://oeis.org/A289146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   search for numbers whose decimal expansion d1…dk is divisible by
 *            sigma0(dk)+sigma1(dk-1)+…+sigma_{k-1}(d1) (sigma_k(n) = sum of
 *            k-th powers of divisors of n)
 * Range:     u8 n <= 21, u16 n <= 225, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#define MAX_K 20   /* maximum digit position needed for 64‑bit. */

seq_t a(idx_t n)
{
    static seq_t *terms = NULL;
    static size_t len   = 0;
    static size_t cap   = 0;
    static seq_t current = (seq_t)1;

    /* Precomputed sigma[d][k] for d = 0…9 and k = 0…MAX_K. */
    static seq_t sigma[10][MAX_K + 1];
    static bool sigma_init = false;

    if (!sigma_init) {
        for (int d = 0; d <= 9; ++d) {
            if (d == 0) {
                for (int k = 0; k <= MAX_K; ++k) sigma[d][k] = (seq_t)0;
            } else {
                /* collect divisors of d */
                int divs[10];
                size_t divcnt = 0;
                for (int i = 1; i <= d; ++i)
                    if (d % i == 0) divs[divcnt++] = i;

                for (int k = 0; k <= MAX_K; ++k) {
                    seq_t sumk = (seq_t)0;
                    for (size_t j = 0; j < divcnt; ++j) {
                        seq_t p = (seq_t)1;
                        for (int t = 0; t < k; ++t) p *= (seq_t)divs[j];
                        sumk += p;
                    }
                    sigma[d][k] = sumk;
                }
            }
        }
        sigma_init = true;
    }

    if (n == 0) return (seq_t)0;   /* not used; protect against 0 */

    /* Generate terms up to the requested index. */
    while (len < (size_t)n) {
        seq_t val  = current;
        seq_t temp = val;
        seq_t sum  = (seq_t)0;
        int pos    = 0;
        bool bad   = false;

        /* compute the required sum and check for zero digits */
        while (temp > (seq_t)0) {
            unsigned int d = (unsigned int)(temp % (seq_t)10);
            if (d == 0) { bad = true; break; }
            sum += sigma[d][pos];
            temp /= (seq_t)10;
            ++pos;
        }

        if (!bad && sum != (seq_t)0 && val % sum == (seq_t)0) {
            /* store the term */
            if (len == cap) {
                size_t new_cap = cap ? cap * 2 : 16;
                seq_t *new_terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!new_terms) exit(1);
                terms = new_terms;
                cap = new_cap;
            }
            terms[len++] = val;
        }
        current += (seq_t)1;   /* next candidate */
    }

    return terms[n - 1];
}
