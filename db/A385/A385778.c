/*
 * A385778  Primes having only {1, 3, 8} as digits.
 * https://oeis.org/A385778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 58, u32 n <= 5665, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes composed only of the decimal digits 1, 3, and 8 */

/* The sequence a(n) returns the n‑th prime (n ≥ 1) whose decimal representation
   uses only digits from the set {1, 3, 8}. */

#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

static const seq_t digits[3] = { (seq_t)1, (seq_t)3, (seq_t)8 };

/* Recursive generator that builds all numbers of a given total length.
   When a full number is constructed it is tested for primality.
   The count of found primes and the value of the target prime are
   updated in the supplied pointers. */
static void
gen_len(idx_t len, seq_t prefix, idx_t pos,
        idx_t *cnt, seq_t *target, idx_t n)
{
    if (*cnt >= n) return;

    if (pos == len) {
        if (isPrimeGeneric(prefix)) {
            ++(*cnt);
            if (*cnt == n) *target = prefix;
        }
        return;
    }

    for (int d = 0; d < 3; ++d) {
        seq_t next = prefix * (seq_t)10 + digits[d];
        /* Guard against overflow on fixed-size types:
           if next < prefix then the multiplication wrapped. */
        if (next < prefix) continue;
        gen_len(len, next, pos + 1, cnt, target, n);
        if (*cnt >= n) return;
    }
}

seq_t a(idx_t n)
{
    idx_t seen = 0;
    seq_t result = 0;

    for (idx_t len = 1; ; ++len) {
        gen_len(len, (seq_t)0, (idx_t)0, &seen, &result, n);
        if (seen >= n) return result;
    }
}
