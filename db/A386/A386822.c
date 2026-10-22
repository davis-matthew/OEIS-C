/*
 * A386822  Irregular table T(n,k) = Product_{j = 1..k} prime(j)^(n-j+1), n >=
 *          0, k = 1..n.
 * https://oeis.org/A386822
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = T(r,k) where T(n,k) = ∏_{j=1..k} prime(j)^(n-j+1) * and
 *            r,k are the row/cell of the triangular array * enumerated
 *            row‑major (row 0 has k=1). * For n = 0 the value is 1 (by
 *            convention)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 33, u128 n <= 52,
 *            big any n
 * Helpers:   build_primes_upto, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"
#include "../utils/seq_pow.h"

static uint64_t *prime_table = NULL;
static size_t prime_count = 0;

/* Build a small prime table once. */
static void init_primes(void)
{
    if (prime_table == NULL) {
        /* Enough for n up to at least 10 000 (row index ≤ 141). */
        build_primes_upto(2000, &prime_table, &prime_count);
    }
}

/* Return the j‑th prime (1‑based).  Assumes init_primes() already ran. */
static seq_t get_prime_u64(uint64_t j)
{
    return (seq_t)prime_table[(size_t)(j - 1)];
}

/* Compute the n‑th term of OEIS A386822. */
seq_t a(idx_t n)
{
    if (n == 0)            /* a(0) = 1 by convention */
        return (seq_t)1;

    init_primes();

    /* Find the smallest r such that r*(r+1)/2 ≥ n  →  row index r. */
    idx_t r = 1;
    for (; ; ++r) {
        if ((r * (r + 1)) / 2 >= n)
            break;
    }

    /* Number of entries before row r : 1 + (r-1)*r/2   (row 0 contributes 1). */
    idx_t before = 1 + ((r - 1) * r) / 2;
    idx_t offset = n - before;          /* 0‑based offset inside the row */
    idx_t k = offset + 1;               /* column index (1‑based) */

    seq_t prod = (seq_t)1;
    for (idx_t j = 1; j <= k; ++j) {
        seq_t base = get_prime_u64(j);
        idx_t exp  = r - j + 1;           /* exponent for this prime */
        seq_t term = seq_pow(base, exp);
        prod *= term;
    }
    return prod;
}
