/*
 * A386207  Numbers m > 1 such that there exists k such that k | m, k^k = k
 *          mod m and 1 < k < m.
 * https://oeis.org/A386207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 168, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A386207: Numbers m > 1 such that there exists an integer k, 1 < k < m,
   with k | m and k^k ≡ k (mod m). The implementation searches
   sequentially for terms up to the required index. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mod_pow.h"

/* Return true iff m satisfies the defining property. */
static bool is_candidate(uint64_t m)
{
    /* All numbers ≡ 2 (mod 4) greater than 2 are automatically in the
       sequence (k = m/2). */
    if ((m & 3ULL) == 2ULL && m > 2ULL) {
        return true;
    }

    /* Test all proper divisors of m except 1 and m itself. */
    for (uint64_t d = 2ULL; d * d <= m; ++d) {
        if (m % d == 0ULL) {
            /* Test divisor d */
            if (mod_pow((seq_t)d, d, (seq_t)m) == (seq_t)d) {
                return true;
            }
            /* Test conjugate divisor e=m/d */
            uint64_t e = m / d;
            if (e != d && e > 1ULL && e < m) {
                if (mod_pow((seq_t)e, e, (seq_t)m) == (seq_t)e) {
                    return true;
                }
            }
        }
    }
    return false;
}

/* Dynamic array of computed sequence values */
static seq_t *terms = NULL;
static size_t terms_len = 0;
static size_t terms_cap = 0;

static void add_term(seq_t v)
{
    if (terms_len == terms_cap) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);          /* Out of memory: abort */
        terms = tmp;
        terms_cap = new_cap;
    }
    terms[terms_len++] = v;
}

/* External linkage function required by the test harness. */
seq_t a(idx_t n)
{
    static uint64_t last_checked = 1ULL;   /* Largest m examined so far */

    while (terms_len < (size_t)n) {
        ++last_checked;
        if (is_candidate(last_checked)) {
            add_term((seq_t)last_checked);
        }
    }
    return terms[n - 1];
}
