/*
 * A325265  Numbers with sum of omega-sequence > 4.
 * https://oeis.org/A325265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth number whose sum of omega‑sequence exceeds 4 (search)
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   append_candidate, bigomega, factor_u64, get_prime, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/bigomega.h"
#include "../utils/factor_u64.h"
#include "../utils/get_prime.h"
#include "../utils/is_prime.h"
#include "../utils/append_candidate.h"

/* Compute red(n):  product of the primes at the positions of the exponents of n */
static uint64_t red_uint64(uint64_t n) {
    /* factor n and obtain exponents */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);
    /* compute product of primes(exponent) */
    unsigned __int128 prod = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint32_t p = get_prime((idx_t)exps[i]);  /* exponent is 1‑based index */
        prod *= p;
    }
    return (uint64_t)prod;
}

/* Return the (a)-th term of the sequence */
seq_t a(idx_t n) {
    if (n == 0) return 0;  /* undefined for offset 0, return 0 */

    static seq_t *terms = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;
    static uint64_t last_checked = 1;   /* last integer examined */

    /* generate terms until we have n of them */
    while (terms_len < (size_t)n) {
        ++last_checked;                     /* candidate integer */
        uint64_t cur = last_checked;
        seq_t sum = 0;
        while (true) {
            /* add Omega(cur) */
            sum += bigomega((idx_t)cur);
            /* stop if cur is prime */
            if (is_prime((idx_t)cur))
                break;
            /* go to next in the omega‑sequence */
            cur = red_uint64(cur);
        }
        if (sum > (seq_t)4) {
            append_candidate(&terms, &terms_len, &terms_cap,
                             (seq_t)last_checked);
        }
    }
    return terms[(size_t)n - 1];
}
