/*
 * A393805  Numbers k for which omega(k)*omega(k + 2)*omega(k + 4)*omega(k +
 *          6)*omega(k + 8) = 2 where omega = A001221.
 * https://oeis.org/A393805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   a(n) = the n-th odd integer k ≥ 1 such that *
 *            ω(k)*ω(k+2)*ω(k+4)*ω(k+6)*ω(k+8) = 2 * where ω counts the
 *            *distinct* prime factors (A001221)
 * Range:     u8 n <= 8, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   count_distinct_primes, ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/count_distinct_primes.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    /* static cache for memoization across calls */
    static seq_t *term_cache = NULL;      /* array of found terms */
    static size_t   term_cap   = 0;       /* current allocation capacity */
    static size_t   term_len   = 0;       /* number of terms stored */
    static idx_t    next_test  = 3;       /* start testing from the first odd ≥ 3 */

    /* return cached value if already computed */
    if (n <= term_len)
        return term_cache[n - 1];

    /* allocate space for at least n terms */
    ensure_capacity(n, &term_cap, (void **)&term_cache, sizeof(seq_t));

    /* find missing terms until we have n of them */
    while (term_len < n) {
        /* Ensure start is odd */
        idx_t start = next_test;
        if ((start & 1) == 0)
            ++start;

        for (idx_t k = start; ; k += 2) {   /* scan only odd numbers */
            int cnt_two = 0;
            bool ok = true;

            for (int i = 0; i < 5; ++i) {
                /* Compute ω for k + 2*i using distinct prime count */
                uint64_t val = (uint64_t)(k + (idx_t)(2 * i));   /* fits in 64‑bit */
                uint64_t w   = count_distinct_primes(val);

                if (w == 0 || w > 2) {      /* product would be 0 or >2 */
                    ok = false;
                    break;
                }
                if (w == 2)
                    ++cnt_two;
            }

            if (ok && cnt_two == 1) {
                /* term found */
                term_cache[term_len++] = (seq_t)k;
                next_test = k + 2;   /* start searching after this term */
                break;
            }
        }
    }

    return term_cache[n - 1];
}
