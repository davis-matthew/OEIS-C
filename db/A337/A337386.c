/*
 * A337386  Numbers k for which A003973(k) >= 2*A003961(k).
 * https://oeis.org/A337386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 13, u32 n <= 9786, u64 n <= 9786,
 *            u128 n <= 9786, big any n
 * Helpers:   append_candidate, factor_u64, next_prime, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by iterating over integers and selecting those whose
   prime‑shift once is an odd abundant number. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"
#include "../utils/sigma_of.h"
#include "../utils/append_candidate.h"

/* Return true if the shift of n, obtained by replacing each prime factor
   p of n by the next prime > p while keeping the exponent, is an odd
   abundant number. */
static bool shift_abundant(idx_t n)
{
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    /* Factor n; primes[i] to exps[i] form the factorization. */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Compute shift(n) in 128‑bit integer to avoid overflow. */
    unsigned __int128 shift128 = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t newp = next_prime(primes[i]);

        /* newp^exps[i] via binary exponentiation. */
        unsigned __int128 base = newp;
        uint64_t e = (uint64_t)exps[i];
        unsigned __int128 pow = 1;
        while (e) {
            if (e & 1)
                pow *= base;
            base *= base;
            e >>= 1;
        }
        shift128 *= pow;
    }
    uint64_t shift = (uint64_t)shift128;          /* fits in 64‑bit for our range */

    seq_t sigma = sigma_of(shift);                /* sum of all divisors */
    seq_t twice = (seq_t)(shift + shift);         /* 2 * shift */
    return sigma > twice;                        /* abundant test */
}

/* Return the n-th term of OEIS sequence A337386 (n ≥ 1). */
seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static size_t len = 0;
    static size_t cap = 0;
    static idx_t cur = 1;        /* next integer to examine */

    while (len < n) {
        while (true) {
            if (shift_abundant(cur)) {
                append_candidate(&seq, &len, &cap, (seq_t)cur);
                ++cur;
                break;
            }
            ++cur;
        }
    }
    return seq[n - 1];
}
