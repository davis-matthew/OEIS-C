/*
 * A248763  Greatest k such that k^3 divides n!
 * https://oeis.org/A248763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 23, u32 n <= 38, u64 n <= 56, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A248763 – Greatest k such that k³ | n!
    a(n) = ∏_{p ≤ n} p^(⌊v_p(n!)/3⌋),  where v_p(n!) = Σ_{i≥1} ⌊n/p^i⌋
    This implementation builds all primes ≤ n with an Eratosthenes sieve
    and multiplies the required prime powers.  Complexity O(n log log n). */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* External types supplied by the build environment */
extern seq_t a(idx_t n);

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  For n = 1 the value is 1. */
    if (n < 2)
        return (seq_t)1;

    size_t len = (size_t)n + 1;           /* array size for the sieve */
    unsigned char *is_comp = (unsigned char *)malloc(len);
    if (!is_comp)
        return (seq_t)1;                  /* allocation failure – return 1 */

    memset(is_comp, 0, len);              /* initialise to prime */

    /* Dynamic array for primes */
    size_t primes_cap = 8, primes_cnt = 0;
    uint64_t *primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
    if (!primes) { free(is_comp); return (seq_t)1; }

    /* Build primes up to n */
    for (idx_t i = 2; i <= n; ++i) {
        if (!is_comp[(size_t)i]) {
            /* i is prime */
            if (primes_cnt == primes_cap) {
                primes_cap *= 2;
                uint64_t *tmp = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
                if (!tmp) { free(is_comp); free(primes); return (seq_t)1; }
                primes = tmp;
            }
            primes[primes_cnt++] = (uint64_t)i;

            if (i <= n / i) {
                for (idx_t j = i * i; j <= n; j += i)
                    is_comp[(size_t)j] = 1;
            }
        }
    }

    seq_t result = (seq_t)1;

    /* For each prime p, compute floor(v_p(n!)/3) and multiply p^(exp) */
    for (size_t pi = 0; pi < primes_cnt; ++pi) {
        uint64_t p = primes[pi];
        idx_t t = n / (idx_t)p;
        idx_t e = 0;
        while (t) {
            e += t;
            t /= (idx_t)p;
        }
        idx_t exp3 = e / 3;
        if (exp3 == 0)
            continue;

        /* Compute p^exp3 by exponentiation by squaring */
        seq_t factor = (seq_t)1;
        seq_t base   = (seq_t)p;
        idx_t ee = exp3;
        while (ee) {
            if (ee & 1)
                factor *= base;
            base *= base;
            ee >>= 1;
        }
        result *= factor;
    }

    free(is_comp);
    free(primes);
    return result;
}
