/*
 * A273290  A273288(n)^Omega(n), where Omega = A001222.
 * https://oeis.org/A273290
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A273288(n)^Omega(n) where A273288(n) is the
 *            median-integer derived from the multiset of prime factors of n:
 *            • If the number of factors (with multiplicity) Ω(n) is odd, it
 *            is the middle prime factor. • If Ω(n) is even, it is the floor
 *            of the average of the two middle primes, replaced by the...
 * Range:     u8 n <= 73, u16 n <= 1041, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, factor_u64, is_prime_big, prev_prime, prime_at_position
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required utilities */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"    /* factor_u64() */
#include "../utils/big_pow.h"      /* big_pow()    */
#include "../utils/is_prime_big.h" /* is_prime_big() */
#include "../utils/prev_prime.h"   /* prev_prime() */
#include "../utils/prime_at_position.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    /* Small fixed workspace sufficient for 64‑bit n. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64(n, primes, exps, &cnt);

    /* Total number of prime factors with multiplicity. */
    size_t omega = 0;
    for (size_t i = 0; i < cnt; ++i)
        omega += (size_t)exps[i];

    /* Determine the base prime A273288(n). */
    uint64_t base;
    if (omega & 1ULL) {        /* odd number of factors */
        size_t mid = omega / 2;
        base = prime_at_position(primes, exps, cnt, mid);
    } else {                   /* even number of factors */
        size_t low_idx  = omega / 2 - 1;
        size_t high_idx = omega / 2;
        uint64_t p_low  = prime_at_position(primes, exps, cnt, low_idx);
        uint64_t p_high = prime_at_position(primes, exps, cnt, high_idx);

        /* Use 128‑bit arithmetic to avoid overflow. */
        unsigned __int128 sum = (unsigned __int128)p_low + p_high;
        uint64_t floor_median = (uint64_t)(sum / 2);

        /* Largest prime ≤ floor_median. */
        if (is_prime_big((seq_t)floor_median))
            base = (uint64_t)floor_median;
        else
            base = (uint64_t)prev_prime((idx_t)floor_median);
    }

    /* Return base^omega.  Use seq_t for arbitrary‑precision cases. */
    return big_pow((seq_t)base, (seq_t)omega);
}
