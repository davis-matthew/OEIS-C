/*
 * A352780  Square array A(n,k), n >= 1, k >= 0, read by descending
 *          antidiagonals, such that the row product is n and column k
 *          contains only (2^k)-th powers of squarefree numbers.
 * https://oeis.org/A352780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) is obtained by reading the square array A(i,j) (defined by
 *            * A(i,0) = product of primes dividing i with odd exponent
 *            (squarefree kernel), * for k>0: A(i,k) = (product of primes p
 *            where exponent in i has bit k set) * raised to the 2^k power)
 *            along descending antidiagonals. * The function a(id...
 * Range:     u8 n <= 9244, u16 n <= 9244, u32 n <= 9244, u64 n <= 9244,
 *            u128 n <= 9244, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* Compute A(i,k) for given row i (>=1) and column k (>=0). */
static seq_t compute_A_ik(idx_t i, idx_t k)
{
    /* Prime factorization of i */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    factor_u64((uint64_t)i, primes, exps, &cnt);

    if (k == 0) {
        seq_t res = 1;
        for (size_t idx = 0; idx < cnt; ++idx) {
            if (exps[idx] & 1ULL)          /* odd exponent */
                res *= (seq_t)primes[idx];
        }
        return res;
    } else {
        if (k >= 64)                      /* bits beyond 63 are zero */
            return 1;

        seq_t squarefree = 1;
        for (size_t idx = 0; idx < cnt; ++idx) {
            if ((exps[idx] >> k) & 1ULL)  /* bit k is set in exponent */
                squarefree *= (seq_t)primes[idx];
        }

        if (squarefree == 1)
            return 1;

        /* Compute squarefree^(2^k) using binary exponentiation. */
        unsigned int e = 1u << (unsigned int)k;   /* 2^k (k<64) */
        seq_t base = squarefree;
        seq_t result = 1;
        while (e) {
            if (e & 1u)
                result *= base;
            base *= base;
            e >>= 1u;
        }
        return result;
    }
}

/* Return the n-th element (1‑based) of the sequence. */
seq_t a(idx_t n)
{
    /* Find the diagonal index s such that s(s+1)/2 >= n */
    idx_t s = 0;
    idx_t sum = 0;
    while (sum < n) {
        ++s;
        sum += s;
    }
    /* i is the row index, j the column index */
    idx_t i = n - (s - 1) * s / 2;
    idx_t j = s - i;

    return compute_A_ik(i, j);
}
