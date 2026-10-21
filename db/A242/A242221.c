/*
 * A242221  Numbers n such that n - n^2/m^2 and 2n - n/m are not prime for all
 *          m.
 * https://oeis.org/A242221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 896, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric, sqrt_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A242221 – numbers n such that for every divisor m of n
 *  n - (n/m)^2 and 2*n - n/m are not prime.  The routine
 *  generates the sequence by testing successive integers.
 *  The search uses only the helper functions isPrimeGeneric
 *  and sqrt_floor provided by the build environment.  */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/sqrt_floor.h"

/* Test whether a given n keeps the conditions for all divisors m. */
static bool passes(seq_t n)
{
    /* Fast-reject using trivial divisors 1 and n when they exist */
    /* 1 is divisor of all n, n is divisor of itself. */

    seq_t limit = sqrt_floor(n);   /* floor(sqrt(n)) */
    seq_t m;

    for (m = 1; m <= limit; ++m) {
        if (n % m == 0) {
            /* m is a divisor */
            seq_t k = n / m;                 /* k = n/m   (integer) */
            seq_t val2 = 2 * n - k;           /* 2n - n/m  */
            if (val2 > 1 && isPrimeGeneric(val2))
                return false;

            /* n - (n/m)^2  is only meaningful if k <= limit,
             * i.e. k*k <= n to avoid unsigned underflow. */
            if (k <= limit) {
                seq_t kk = k * k;
                seq_t val1 = n - kk;          /* n - (n/m)^2 */
                if (val1 > 1 && isPrimeGeneric(val1))
                    return false;
            }

            /* n/m is the complementary divisor > limit
             * (unless m == n/m). */
            seq_t m2 = n / m;                 /* m2 = n/m   */
            if (m2 != m) {
                seq_t k2 = n / m2;            /* k2 = n/m2 = m */
                seq_t val2_2 = 2 * n - k2;
                if (val2_2 > 1 && isPrimeGeneric(val2_2))
                    return false;

                if (k2 <= limit) {
                    seq_t kk2 = k2 * k2;
                    seq_t val1_2 = n - kk2;
                    if (val1_2 > 1 && isPrimeGeneric(val1_2))
                        return false;
                }
            }
        }
    }
    return true;
}

/* Main sequence function.  Cached results enable non‑sequential calls. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                         /* undefined for indices < 1 */

    static seq_t *terms = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;
    static seq_t next_candidate = 1;

    while (terms_len < (size_t)n) {
        seq_t cand = next_candidate;
        if (passes(cand)) {
            if (terms_len >= terms_cap) {
                terms_cap = (terms_cap ? terms_cap * 2 : 8);
                seq_t *tmp = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
                if (!tmp) {                /* allocation failure – abort */
                    if (terms) free(terms);
                    return 0;
                }
                terms = tmp;
            }
            terms[terms_len++] = cand;
        }
        ++next_candidate;
    }
    return terms[n - 1];
}
