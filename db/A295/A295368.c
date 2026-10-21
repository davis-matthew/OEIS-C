/*
 * A295368  For any number n > 0 with s divisors, say d_1, d_2, ..., d_s such
 *          that d_1 = 1 < d_2 < ... < d_s = n, the binary representation of
 *          a(n) is (d_1 mod 2, d_2 mod 2, ..., d_s mod 2).
 * https://oeis.org/A295368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = binary number with bit i = (d_i mod 2) where d_1 < d_2 <
 *            … < d_s are the divisors of n in ascending order
 * Range:     u8 n <= 35, u16 n <= 179, u32 n <= 1259, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Comparator for qsort over uint64_t */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

/* Compute the A295368 value for index n (n > 0). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* defiance of the domain, harmless */

    /* -------- 1. Factorise n --------------------------------- */
    idx_t temp = n;
    uint64_t primes[64];
    int exps[64];
    size_t pf = 0;

    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[pf] = p;
            exps[pf]   = e;
            ++pf;
        }
    }
    if (temp > 1) {                 /* remaining prime factor */
        primes[pf] = temp;
        exps[pf]   = 1;
        ++pf;
    }

    /* -------- 2. Build all divisors -------------------------------- */
    size_t cap = 64;                 /* small initial capacity */
    uint64_t *divs = (uint64_t *)malloc(cap * sizeof(uint64_t));
    if (!divs) return 0;             /* unlikely, but graceful */

    divs[0] = 1;
    size_t sz = 1;

    for (size_t i = 0; i < pf; ++i) {
        uint64_t p = primes[i];
        int e = exps[i];
        size_t old_sz = sz;
        uint64_t mult = 1;
        for (int k = 1; k <= e; ++k) {
            mult *= p;
            for (size_t j = 0; j < old_sz; ++j) {
                if (sz == cap) {
                    cap *= 2;
                    uint64_t *tmp = (uint64_t *)realloc(divs, cap * sizeof(uint64_t));
                    if (!tmp) {
                        free(divs);
                        return 0;
                    }
                    divs = tmp;
                }
                divs[sz++] = divs[j] * mult;
            }
        }
    }

    /* -------- 3. Sort divisors ------------------------------------- */
    qsort(divs, sz, sizeof(uint64_t), cmp_uint64);

    /* -------- 4. Build the binary number ----------------------------- */
    seq_t res = 0;
    for (size_t i = 0; i < sz; ++i) {
        res <<= 1;
        res |= (seq_t)(divs[i] & 1ULL);
    }

    free(divs);
    return res;
}
