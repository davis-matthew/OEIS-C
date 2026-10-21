/*
 * A258838  Practical numbers q with q-1 and q+1 twin primes: "Sandwiches of
 *          the second kind".
 * https://oeis.org/A258838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 474, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate, factor_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th term of OEIS A258838:
   practical numbers q such that q-1 and q+1 are twin primes.  */

/* External helper functions (headers provided by the build system). */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime.h"
#include "../utils/factor_u64.h"
#include "../utils/append_candidate.h"

/* Helper: test whether a positive integer q (q ≥ 4) is a practical number. */
static bool is_practical(idx_t q)
{
    if (q <= 2) return false;         /* practical numbers >2 are even */
    if (q & 1) return false;          /* odd numbers cannot be practical */

    /* Factor q into primes (assumed sorted ascending). */
    uint64_t primes_arr[20];
    uint64_t exps_arr[20];
    size_t cnt = 0;
    factor_u64((uint64_t)q, primes_arr, exps_arr, &cnt);

    if (cnt == 0 || primes_arr[0] != 2u) return false;  /* must start with 2 */

    seq_t sigma_so_far = 1;           /* σ of processed prime powers */

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes_arr[i];
        int e = (int)exps_arr[i];

        /* Compute σ(p^e) = 1 + p + p^2 + … + p^e. */
        seq_t sigma_p = 1;
        seq_t term = 1;
        for (int k = 0; k < e; ++k) {
            term = term * p;
            sigma_p += term;
        }

        if (i > 0) {  /* for primes after the first, apply the practical number test */
            if (p > sigma_so_far + 1) return false;
        }

        sigma_so_far = sigma_so_far * sigma_p;
    }
    return true;
}

/* Main sequence routine. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t last_checked = 3;           /* last number examined; start just below 4 */

    if (n == 0) return 0;                   /* undefined for n < 1, harmless */

    /* Return cached value if already computed. */
    if (cache_len >= (size_t)n)
        return cache[(size_t)n - 1];

    /* Incrementally search for the next qualifying numbers. */
    while (cache_len < (size_t)n) {
        ++last_checked;
        if (last_checked < 4) continue;     /* first term is 4 */

        /* Practical number test. */
        if (!is_practical(last_checked))
            continue;

        /* Twin prime neighbors test. */
        if (!is_prime(last_checked - 1) || !is_prime(last_checked + 1))
            continue;

        /* Store the discovered term. */
        append_candidate(&cache, &cache_len, &cache_cap, (seq_t)last_checked);
    }

    return cache[(size_t)n - 1];
}
