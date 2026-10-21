/*
 * A279060  Number of divisors of n of the form 6*k + 1.
 * https://oeis.org/A279060
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of divisors d of n with d ≡ 1 (mod 6)
 * Range:     u8 n <= 8693, u16 n <= 8693, u32 n <= 8693, u64 n <= 8693,
 *            u128 n <= 8693, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function counts the admissible divisors by factorizing n.
   For each prime factor:
     – 2 or 3 may not appear in a divisor, so they are ignored.
     – a prime p ≡ 1 (mod 6) contributes (e+1) choices (any exponent).
     – a prime p ≡ 5 (mod 6) contributes a restricted set:
       an exponent k is admissible iff the total parity of all such
       exponents is even.  This is handled by computing
       (#even‐exponent choices + #odd‐exponent choices)^∏
       and halving plus a correction term.

   The computation uses only integer arithmetic and works for
   unsigned 8/16/32/64‑bit seq_t or arbitrary‑precision seq_t. */

#include "../utils/factor_u64.h"
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0ULL;                /* a(0) is defined to be 0 */

    /* Factor n into prime powers.  The helper expects uint64_t arrays. */
    static uint64_t primes[64];
    static uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t prod1 = 1;                 /* product for primes ≡ 1 (mod 6) */
    seq_t prod5 = 1;                 /* product of (a+b) for primes ≡ 5 (mod 6) */
    bool anyOdd = false;             /* true if any 5‑prime exponent is odd */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        if (p == 2 || p == 3)        /* divisors containing 2 or 3 are invalid */
            continue;

        uint64_t rem = p % 6;
        if (rem == 1) {
            prod1 *= (seq_t)(e + 1);
        } else if (rem == 5) {
            seq_t a = (seq_t)(e / 2 + 1);            /* count of even exponents */
            seq_t b = (seq_t)((e + 1) / 2);          /* count of odd  exponents */
            prod5 *= (a + b);
            if (e & 1ULL)
                anyOdd = true;
        }
        /* No other residues occur for primes > 3. */
    }

    /* Count assignments for 5‑prime exponents with even total parity. */
    seq_t diff = anyOdd ? (seq_t)0 : (seq_t)1;
    seq_t count5 = (prod5 + diff) / 2;

    /* Final result: combine choices from 1‑ and 5‑prime categories. */
    return prod1 * count5;
}
