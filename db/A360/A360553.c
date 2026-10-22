/*
 * A360553  Numbers > 1 whose unordered prime signature has integer median.
 * https://oeis.org/A360553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th integer >1 whose unordered prime signature has integer median. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* Maximum number of distinct prime factors expected in a 64‑bit integer */
#define MAX_FACTORS 64

seq_t a(idx_t n)
{
    /* Special case: the first term is 2. */
    if (n == 1ULL) return (seq_t)2;

    idx_t   counted = 0;          /* how many terms found so far          */
    uint64_t candidate = 2;       /* current integer to test             */

    int exps[MAX_FACTORS];        /* exponent multiset for factorization */

    while (counted < n) {
        uint64_t tmp = candidate;
        size_t   k   = 0;         /* number of distinct primes            */

        /* factorization by trial division */
        if ((tmp & 1ULL) == 0) {          /* divisible by 2 */
            int e = 0;
            do { tmp >>= 1; e++; } while ((tmp & 1ULL) == 0);
            exps[k++] = e;
        }
        for (uint64_t p = 3; p * p <= tmp; p += 2) {
            if (tmp % p == 0) {
                int e = 0;
                do { tmp /= p; e++; } while (tmp % p == 0);
                exps[k++] = e;
            }
        }
        if (tmp > 1) {                    /* remaining prime factor */
            exps[k++] = 1;
        }

        bool ok;

        /* If number of distinct primes is odd, median is integer. */
        if (k & 1) {
            ok = true;
        } else {
            /* Need the two middle exponents to have the same parity. */
            /* Sort the exponents (small array, insertion sort). */
            for (size_t i = 1; i < k; ++i) {
                int v = exps[i];
                size_t j = i;
                while (j > 0 && exps[j - 1] > v) {
                    exps[j] = exps[j - 1];
                    --j;
                }
                exps[j] = v;
            }
            int a1 = exps[(k / 2) - 1];
            int a2 = exps[k / 2];
            ok = ((a1 + a2) & 1) == 0;
        }

        if (ok) {
            ++counted;
            if (counted == n) return (seq_t)candidate;
        }

        ++candidate;
    }

    /* Unreachable but keeps the compiler happy. */
    return (seq_t)0;
}
