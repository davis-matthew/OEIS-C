/*
 * A340325  Numbers k such that starting with k and repeatedly applying the
 *          map x -> A340323(x) reaches the fixed point 12.
 * https://oeis.org/A340325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   is_power_of_two, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A340325:
   Numbers k such that starting with k and repeatedly applying the map
   x -> A340323(x) reaches the fixed point 12.
   Equivalent: k is not a power of two and (there is a non‑Mersenne prime
   factor) or (k is divisible by the square of a Mersenne prime > 3). */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_power_of_two.h"
#include "../utils/is_prime_big.h"

/* Return true iff p is a Mersenne prime and p > 3. */
static bool is_mersenne_prime(seq_t p)
{
    if (p <= (seq_t)3) return false;
    return is_power_of_two(p + (seq_t)1) && is_prime_big(p);
}

/* Test whether a candidate satisfies the definition of the sequence. */
static bool qualifies(seq_t x)
{
    /* Exclude powers of two. */
    if (is_power_of_two(x)) return false;

    uint64_t num   = (uint64_t)x;
    uint64_t temp  = num;
    uint64_t d     = 2;

    while (d * d <= temp) {
        if (temp % d == 0) {
            int exp = 0;
            while (temp % d == 0) {
                temp /= d;
                exp++;
            }

            if (d == 2)   /* 2 is a non‑Mersenne prime factor. */
                return true;

            if (d == 3)   /* 3 is a Mersenne prime ≤ 3; ignore. */
                continue;

            if (is_mersenne_prime((seq_t)d) && d > 3) {
                if (exp >= 2)      /* square of a Mersenne prime > 3. */
                    return true;
                /* exponent 1 does not qualify. */
            } else {               /* non‑Mersenne prime factor. */
                return true;
            }
        }
        d += (d == 2 ? 1 : 2);  /* 2 → 3 → 5 → 7 → … */
    }

    /* Remaining factor > 1 is prime. */
    if (temp > 1) {
        if (temp == 2)                return true;
        if (temp == 3)                return false; /* 3≤3 */
        if (is_mersenne_prime((seq_t)temp) && temp > 3)
            return false;             /* exponent 1 of Mersenne >3 */
        return true;                 /* non‑Mersenne prime factor. */
    }
    return false;                     /* all factors were excluded. */
}

/* Return the nth term (1‑based). */
seq_t a(idx_t n)
{
    idx_t  count = 0;
    seq_t  cand  = (seq_t)5;          /* first term in the sequence. */

    while (1) {
        if (qualifies(cand)) {
            count++;
            if (count == n)
                return cand;
        }
        cand++;
    }
}
