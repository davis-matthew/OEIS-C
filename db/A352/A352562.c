/*
 * A352562  a(n) = number of modules with n elements over the ring of integers
 *          in the imaginary quadratic field of discriminant -4.
 * https://oeis.org/A352562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = number of Z[i]-modules of order n, multiplicative over n
 *            using local prime‑power factors
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Precompute partition numbers up to exponent 64. */
static seq_t part[65];
static bool part_inited = false;

static void init_partitions(void)
{
    if (part_inited) return;
    part_inited = true;
    part[0] = (seq_t)1;
    for (int i = 1; i < 65; ++i)
        part[i] = (seq_t)0;
    for (int i = 1; i < 65; ++i)
        for (int j = i; j < 65; ++j)
            part[j] += part[j - i];
}

/* Return the number of Z[i]-modules of order n. */
seq_t a(idx_t n)
{
    init_partitions();

    uint64_t m = (uint64_t)n;            /* work with 64‑bit integer */
    seq_t result = (seq_t)1;             /* multiplicative identity */

    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p != 0) continue;
        int exp = 0;
        while (m % p == 0) {
            m /= p;
            ++exp;
        }

        /* Contribution of prime p */
        if (p == 2) {                       /* ramified prime */
            result *= part[exp];
        } else if ((p & 3) == 3) {          /* inert prime: p ≡ 3 mod 4 */
            if (exp & 1) return (seq_t)0;          /* odd exponent impossible */
            int e = exp >> 1;               /* exponent of the ideal */
            result *= part[e];
        } else {                            /* split prime: p ≡ 1 mod 4 */
            seq_t sum = (seq_t)0;
            for (int k = 0; k <= exp; ++k)
                sum += part[k] * part[exp - k];
            result *= sum;
        }
    }

    /* Remaining prime factor > sqrt(m) if any */
    if (m > 1) {
        uint64_t p = m;
        int exp = 1;
        if (p == 2) {
            result *= part[exp];
        } else if ((p & 3) == 3) {
            /* exp is 1, odd → no module possible */
            return (seq_t)0;
        } else { /* split prime */
            seq_t sum = (seq_t)0;
            for (int k = 0; k <= exp; ++k)
                sum += part[k] * part[exp - k];
            result *= sum;
        }
    }

    return result;
}
