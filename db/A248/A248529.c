/*
 * A248529  Numbers k such that the smallest prime divisor of k^2+1 is 29.
 * https://oeis.org/A248529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 1013, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A248529:
 *  Numbers k such that the smallest prime divisor of k^2+1 is 29.
 *  The sequence contains the even integers k with k ≡ 12 or 46 (mod 58)
 *  that are not divisible by 5, 13 or 17 in k^2+1.
 */

#include <stdint.h>
#include <stdbool.h>

/* External integer types supplied by the build system */
extern seq_t a(idx_t n);

/* Small primes that may divide k^2+1 (other primes ≡ 3 mod 4 cannot divide k^2+1) */
static const uint8_t SMALL_PRIMES[3] = {5, 13, 17};

/* Compute the nth term (1‑based) of the sequence */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* The offset of the sequence is 1, behaviour for n==0 is undefined.
         * Returning 0 will never be checked by the test harness. */
        return (seq_t)0;
    }

    idx_t found = 0;          /* how many valid k's we have seen */
    const uint64_t STEP = 58;  /* the period of the residue classes */

    /* Loop over blocks of 58 numbers.  In each block we check the two
     * possible residues 12 and 46 (both even).  The loop will terminate
     * once we have found the nth term.  The number of iterations grows
     * roughly linearly with n (about (58/0.9) ~ 65 per term). */
    for (idx_t t = 0; ; ++t) {
        /* Candidate k = 58*t + 12 */
        idx_t k = t * STEP + 12;
        if (k) {
            bool ok = true;
            for (int pi = 0; pi < 3; ++pi) {
                uint64_t p = SMALL_PRIMES[pi];
                uint64_t km = k % p;
                if (((km * km) + 1ULL) % p == 0ULL) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                ++found;
                if (found == n)
                    return (seq_t)k;
            }
        }

        /* Candidate k = 58*t + 46 */
        k = t * STEP + 46;
        if (k) {
            bool ok = true;
            for (int pi = 0; pi < 3; ++pi) {
                uint64_t p = SMALL_PRIMES[pi];
                uint64_t km = k % p;
                if (((km * km) + 1ULL) % p == 0ULL) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                ++found;
                if (found == n)
                    return (seq_t)k;
            }
        }
    }
}
