/*
 * A392448  The sum of the exponential divisors of n that are numbers whose
 *          prime factorization exponents are all powers of 2 (A138302).
 * https://oeis.org/A392448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = product over prime powers p^e||n of (sum_{k=0..v2(e)}
 *            p^(2^k))
 * Range:     u8 n <= 143, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/*
 * Factorize a uint64_t into its distinct prime factors and exponents.
 * Output: primes[0..c-1], exps[0..c-1], count in *cnt.
 * Assumes arrays have space for at least 64 entries.
 */
static void factorize_uint64(uint64_t n, uint64_t *primes, int *exps, size_t *cnt) {
    size_t c = 0;
    if (n <= 1) {
        *cnt = 0;
        return;
    }
    uint64_t m = n;
    int e = 0;
    while ((m & 1) == 0) {
        e++;
        m >>= 1;
    }
    if (e > 0) {
        primes[c] = 2;
        exps[c]   = e;
        c++;
    }
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            e = 0;
            while (m % p == 0) {
                m /= p;
                e++;
            }
            primes[c] = p;
            exps[c]   = e;
            c++;
        }
    }
    if (m > 1) {          /* remaining prime factor */
        primes[c] = m;
        exps[c]   = 1;
        c++;
    }
    *cnt = c;
}

/* Main function requested by the assignment */
seq_t a(idx_t n) {
    uint64_t nn = (uint64_t)n;
    /* handle 1 explicitly – it has no prime factors */
    if (nn <= 1)
        return (seq_t)1;

    /* Temporary storage for factorization */
    uint64_t primes[64];
    int      exps[64];
    size_t   cnt;

    factorize_uint64(nn, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        seq_t   p     = (seq_t)primes[i];
        int     e     = exps[i];

        /* compute t = v2(e) */
        int t = 0;
        int tmp = e;
        while ((tmp & 1) == 0) {
            ++t;
            tmp >>= 1;
        }

        /* term = sum_{k=0..t} p^(2^k) */
        seq_t term  = (seq_t)0;
        seq_t pow   = p;            /* p^(2^0) = p^1 */
        for (int k = 0; k <= t; ++k) {
            term += pow;
            pow   = pow * pow;      /* next power is square of the previous */
        }

        result *= term;
    }

    return result;
}
