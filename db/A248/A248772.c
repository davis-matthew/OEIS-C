/*
 * A248772  Greatest 6th-power-free divisor of n!.
 * https://oeis.org/A248772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏_{p≤n} p^(v_p(n!) mod 6), where v_p(n!) = Σ_{k≥1}
 *            ⌊n/p^k⌋
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 33, u128 n <= 58,
 *            big any n
 * Helpers:   build_primes_upto, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the greatest 6th‑power‑free divisor of n!. */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/pow_seq.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n) {
    static uint64_t *primes = NULL;
    static size_t prime_count = 0;
    static idx_t prime_limit = 0;

    if (n < 2)              /* 1! = 1 */
        return (seq_t)1;

    if (n > prime_limit) { /* regenerate primes up to new n */
        free(primes);
        build_primes_upto(n, &primes, &prime_count);
        prime_limit = n;
    }

    seq_t result = 1;
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = primes[i];
        if (p > n)
            break;

        idx_t t = n;
        int mod = 0;
        while (t) {           /* compute v_p(n!) mod 6 */
            t /= p;
            mod = (mod + t) % 6;
        }
        if (mod == 0)
            continue;

        seq_t p_pow = pow_seq((seq_t)p, mod); /* p^mod */
        result *= p_pow;
    }
    return result;
}
