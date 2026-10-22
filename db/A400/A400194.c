/*
 * A400194  Maximum number of terms in a 2-dense sublist of divisors of n.
 * https://oeis.org/A400194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, new
 * Formula:   a(n) = maximum number of terms in a 2‑dense sublist of divisors
 *            of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Compare function for qsort on uint64_t */
static int cmp_uint64(const void *a, const void *b) {
    uint64_t va = *(const uint64_t *)a, vb = *(const uint64_t *)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

/* Return the maximum number of terms in a 2‑dense sublist of
   the sorted list of divisors of n. */
seq_t a(idx_t n) {
    if (n < 1) return 0;

    /* Factorisation: collect primes and exponents */
    uint64_t primes[64];
    int exps[64];
    size_t fp = 0;
    uint64_t m = (uint64_t)n;

    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            primes[fp] = p;
            exps[fp] = e;
            ++fp;
        }
    }
    if (m > 1) {
        primes[fp] = m;
        exps[fp] = 1;
        ++fp;
    }

    /* Generate all divisors (unsorted) */
    size_t div_cnt = 1;
    uint64_t *divs = (uint64_t *)malloc(div_cnt * sizeof(uint64_t));
    if (!divs) return 0;
    divs[0] = 1;

    for (size_t i = 0; i < fp; ++i) {
        uint64_t p = primes[i];
        int e = exps[i];
        size_t new_cnt = div_cnt * (e + 1);
        uint64_t *new_divs = (uint64_t *)malloc(new_cnt * sizeof(uint64_t));
        if (!new_divs) {
            free(divs);
            return 0;
        }
        size_t idx = 0;
        for (size_t j = 0; j < div_cnt; ++j) {
            uint64_t val = 1;
            for (int k = 0; k <= e; ++k) {
                new_divs[idx++] = divs[j] * val;
                val *= p;
            }
        }
        free(divs);
        divs = new_divs;
        div_cnt = new_cnt;
    }

    /* Sort the divisors */
    qsort(divs, div_cnt, sizeof(uint64_t), cmp_uint64);

    /* Find longest contiguous 2‑dense subsequence */
    size_t max_len = 1, cur_len = 0;
    for (size_t i = 0; i < div_cnt; ++i) {
        if (i == 0 || divs[i] <= divs[i - 1] * 2ULL)
            ++cur_len;
        else
            cur_len = 1;
        if (cur_len > max_len) max_len = cur_len;
    }

    free(divs);
    return (seq_t)max_len;
}
