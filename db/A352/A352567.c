/*
 * A352567  a(n) = number of modules with n elements over the ring of integers
 *          in the imaginary quadratic field of discriminant -19.
 * https://oeis.org/A352567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) counts the R‑modules of order n, where R is the ring of
 *            integers of Q(√‑19). The sequence is multiplicative; for a prime
 *            power p^k | n the contribution is: * if p = 2 (inert): k must be
 *            even, contribution = p(k/2) * if p = 19 (ramified): contribution
 *            = p(k) * if Legendre(-19/p) = 1 (split):...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   init_partitions, jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/jacobi_symbol.h"
#include "../utils/init_partitions.h"

/* ------------------------------------------------------------ */
/* Partition numbers  p(0)=1, p(n)=sum_{k=1}∞ (-1)^{k-1}
   [p(n‑k(3k‑1)/2)+p(n‑k(3k+1)/2)]  (Euler's recurrence).  */
#define PART_MAX 64   /* enough for n≤10000 (exponents ≤13) */

/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for offset 1, but harmless */

    /* Lazily initialize the partition table once. */
    static seq_t part[PART_MAX];
    static int part_inited = 0;
    if (!part_inited) {
        init_partitions(part, PART_MAX);
        part_inited = 1;
    }

    seq_t result = 1;
    idx_t temp = n;

    /* ----- handle prime 2 (always inert) ----- */
    if ((temp & 1) == 0) {                     /* 2 divides n? */
        idx_t exp = 0;
        while ((temp & 1) == 0) {
            temp >>= 1;
            ++exp;
        }
        if (exp & 1) return 0;                /* odd exponent → impossible */
        result *= part[(size_t)(exp / 2)];
    }

    /* ----- factor the remaining odd part ----- */
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p != 0) continue;

        idx_t exp = 0;
        while (temp % p == 0) {
            temp /= p;
            ++exp;
        }

        if (p == 19) {                        /* ramified prime */
            result *= part[(size_t)exp];
            continue;
        }

        /* Legendre symbol (-19|p) */
        uint64_t a = (p - (19 % p)) % p;       /* (-19) mod p        */
        int symbol = jacobi_symbol(a, p);

        if (symbol == 1) {                    /* split */
            seq_t conv = 0;
            for (idx_t i = 0; i <= exp; ++i)
                conv += part[(size_t)i] * part[(size_t)(exp - i)];
            result *= conv;
        } else if (symbol == -1) {            /* inert */
            if (exp & 1) return 0;            /* odd exponent impossible */
            result *= part[(size_t)(exp / 2)];
        } else {                              /* symbol == 0, should not happen */
            return 0;
        }
    }

    /* ----- residual prime factor > sqrt(n) (exponent 1) ----- */
    if (temp > 1) {
        idx_t exp = 1;
        if (temp == 19) {                    /* ramified */
            result *= part[exp];
        } else {
            uint64_t a = (temp - (19 % temp)) % temp;
            int symbol = jacobi_symbol(a, temp);

            if (symbol == 1) {                /* split */
                seq_t conv = 0;
                for (idx_t i = 0; i <= exp; ++i)
                    conv += part[(size_t)i] * part[(size_t)(exp - i)];
                result *= conv;
            } else if (symbol == -1) {        /* inert, exponent 1 odd → 0 */
                return 0;
            } else {
                return 0;                    /* unexpected case */
            }
        }
    }

    return result;
}
