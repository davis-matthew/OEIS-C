/*
 * A352564  a(n) = number of modules with n elements over the ring of integers
 *          in the imaginary quadratic field of discriminant -8.
 * https://oeis.org/A352564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e∥n} f_p(e) where f_2(e) = partition(e) f_p(e) (p
 *            splits) = ∑_{i=0}^{e} partition(i)·partition(e−i) f_p(e) (p
 *            inert) = 0 if e odd, else partition(e/2) (p splits ⇔ p ≡ 1 or 3
 *            (mod 8), p inert ⇔ p ≡ 5 or 7 (mod 8)) This follows from the
 *            structure of finite torsion modules over the PID ℤ...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Precompute partition numbers up to a fixed bound. */
    static bool init_part = false;
    static seq_t *part = NULL;
    static size_t part_max = 0;

    if (!init_part) {
        /* The largest exponent needed for n ≤ 10 000 is ⌊log₂ 10 000⌋ = 13.
           We are conservative and compute up to 200. */
        part_max = 200;
        part = (seq_t *)malloc((part_max + 1) * sizeof(seq_t));
        for (size_t i = 0; i <= part_max; ++i)
            part[i] = 0;
        part[0] = 1;  /* empty partition */

        /* Coin‑change DP: for each coin value (part size) add it to sums. */
        for (size_t coin = 1; coin <= part_max; ++coin) {
            for (size_t sum = coin; sum <= part_max; ++sum) {
                part[sum] += part[sum - coin];
            }
        }
        init_part = true;
    }

    /* Factor n and accumulate the product over prime factors. */
    idx_t temp = n;
    seq_t result = 1;

    /* Handle factor 2 separately. */
    size_t e2 = 0;
    while ((temp & 1) == 0) {
        temp >>= 1;
        ++e2;
    }
    if (e2 > 0) {
        result *= part[e2];
    }

    /* Trial division for odd primes. */
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            size_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }

            seq_t val = 0;
            if ((p & 7ULL) == 1 || (p & 7ULL) == 3) {     /* splits */
                for (size_t i = 0; i <= e; ++i) {
                    val += part[i] * part[e - i];
                }
            } else {                                      /* inert */
                if (e & 1)          /* odd exponent → impossible */
                    return 0;
                val = part[e >> 1];
            }
            result *= val;
        }
    }

    /* Remaining prime factor (if any). */
    if (temp > 1) {            /* temp is prime */
        size_t e = 1;
        seq_t val = 0;
        if ((temp & 7ULL) == 1 || (temp & 7ULL) == 3) {     /* splits */
            /* e == 1: sum_{i=0}^{1} part[i]·part[1−i] = 2·part[1] = 2 */
            for (size_t i = 0; i <= e; ++i)
                val += part[i] * part[e - i];
        } else {                                        /* inert */
            /* e == 1 → odd, so the term is 0 */
            return 0;
        }
        result *= val;
    }

    return result;
}
