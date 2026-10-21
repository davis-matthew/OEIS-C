/*
 * A248707  a(n) = f(3*n)/(f(n-1)*f(n)*f(n+1)), where f(k) = k!.
 * https://oeis.org/A248707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A248707
   Formula: a(n) = (3*n)! / ((n-1)! * n! * (n+1)!)
   Recurrence used: a(1)=3,
   n*(n+2)*a(n+1) = 3*(3*n+1)*(3*n+2)*a(n)   (proof: multinomial coefficient)
*/

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/pow_uint.h"   /* pow_uint(uint64_t base, idx_t exp) returns seq_t */

static inline idx_t exp_in_factorial(idx_t m, idx_t p) {
    idx_t e = 0;
    while (m) {
        m /= p;
        e += m;
    }
    return e;
}

/* cached primes up to 3*max_n */
static idx_t  max_n_cached = 0;
static uint64_t *prime_list = NULL;
static size_t  prime_count = 0;

static void recompute_primes(idx_t n) {
    size_t limit = 3 * n;
    bool *is_comp = (bool*)malloc((limit + 1) * sizeof(bool));
    if (!is_comp) return;
    memset(is_comp, 0, (limit + 1) * sizeof(bool));

    free(prime_list);
    prime_list = (uint64_t*)malloc((limit + 1) * sizeof(uint64_t));
    if (!prime_list) { free(is_comp); return; }

    prime_count = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            prime_list[prime_count++] = i;
            if (i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i)
                    is_comp[j] = true;
            }
        }
    }
    free(is_comp);
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;       /* out of range, undefined for this sequence */

    if (n > max_n_cached) {
        recompute_primes(n);
        max_n_cached = n;
    }

    if (n == 1) return (seq_t)3;

    seq_t result = (seq_t)1;
    idx_t m3 = 3 * n;
    idx_t mn = n;
    idx_t m1 = n - 1;
    idx_t mp1 = n + 1;

    for (size_t idx = 0; idx < prime_count; ++idx) {
        idx_t p = prime_list[idx];
        idx_t exp = exp_in_factorial(m3, p)
                  - exp_in_factorial(m1, p)
                  - exp_in_factorial(mn, p)
                  - exp_in_factorial(mp1, p);
        if (exp > 0) {
            seq_t term = pow_uint((uint64_t)p, exp); /* term = p^exp */
            result = result * term;
        }
    }
    return result;
}
