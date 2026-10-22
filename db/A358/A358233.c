/*
 * A358233  Number of ways n can be expressed as an unordered product of two
 *          natural numbers that do not generate any carries when added
 *          together in the primorial base.
 * https://oeis.org/A358233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3595, u16 n <= 9769, u32 n <= 9769, u64 n <= 9769,
 *            u128 n <= 9769, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computes the OEIS sequence A358233:
 * a(n) = number of unordered factorizations n = d * e (d ≤ e) for which
 * the addition d + e written in primorial base has no carries.
 * The function enumerates all divisors d ≤ √n and checks the
 * carry‑free condition via mixed–radix digits (base 2,3,5,7,…).
 */

#include <stdbool.h>
#include <stddef.h>

/* first 20 primes are sufficient for any 64‑bit integer */
static const unsigned long long PRIMES[20] = {
    2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL,
    23ULL, 29ULL, 31ULL, 37ULL, 41ULL, 43ULL, 47ULL,
    53ULL, 59ULL, 61ULL, 67ULL, 71ULL
};

/* returns true iff the addition of a and b in primorial base 
   never generates a carry.  The function works for any values
   that fit into seq_t (and are not larger than 2^64−1). */
static bool primorial_no_carry(seq_t a, seq_t b)
{
    size_t i = 0;
    /* loop until both numbers become 0, using successive primes */
    while (a > 0 || b > 0) {
        unsigned long long p = PRIMES[i++];
        unsigned long long da = (unsigned long long)(a % p);
        unsigned long long db = (unsigned long long)(b % p);
        if (da + db >= p)
            return false;
        a /= p;
        b /= p;
        /* PRIMES is long enough for any 64‑bit integer */
    }
    return true;
}

/* count unordered factor pairs with carry‑free addition */
seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t e = n / d;
            /* ensure d ≤ e */
            if (d > e)
                break;
            if (primorial_no_carry((seq_t)d, (seq_t)e))
                ++count;
        }
    }
    return count;
}
