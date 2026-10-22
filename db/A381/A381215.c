/*
 * A381215  Numbers k such that the difference between the largest and
 *          smallest element of the set of bases and exponents (including
 *          exponents = 1) in the prime factorization of k is 1.
 * https://oeis.org/A381215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 10, u32 n <= 12, u64 n <= 16, u128 n <= 22,
 *            big any n
 * Helpers:   big_pow, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A381215.
   The terms are generated from the pattern
   - first eight terms are fixed: 2, 8, 9, 36, 72, 81, 108, 216
   - for each prime p ≥ 5 the two terms p^(p-1) and p^(p+1) follow in order.
   The sequence is produced in increasing order. */

/* Include necessary headers from the utilities. */
#include <stdbool.h>
#include "../utils/big_pow.h"        /* big_pow(seq_t, seq_t) */
#include "../utils/build_primes_upto.h" /* build_primes_upto(idx_t, uint64_t **, size_t*) */

seq_t a(idx_t n)
{
    /* Static initialization */
    static bool initialized = false;
    static seq_t base_terms[8];
    static uint64_t *prime_list = NULL;
    static size_t prime_cnt = 0;

    if (!initialized) {
        /* Pre‑computed first eight terms */
        base_terms[0] = (seq_t)2;
        base_terms[1] = (seq_t)8;
        base_terms[2] = (seq_t)9;
        base_terms[3] = (seq_t)36;
        base_terms[4] = (seq_t)72;
        base_terms[5] = (seq_t)81;
        base_terms[6] = (seq_t)108;
        base_terms[7] = (seq_t)216;

        /* Generate sufficient primes
           60000 exceeds the 5 000th prime (≈48611) which is enough for n ≤ 10000. */
        build_primes_upto(60000, (uint64_t **)&prime_list, &prime_cnt);
        initialized = true;
    }

    if (n <= 8)
        return base_terms[n - 1];

    /* Determine which prime and exponent are needed. */
    idx_t m = n - 8;                 /* 1‑based index into the prime‑pairs section */
    idx_t pair_idx = (m - 1) / 2;    /* which prime (0‑based) after 5 */
    idx_t offset   = (m - 1) % 2;    /* 0 → exponent p-1, 1 → exponent p+1 */

    /* primes[2] = 5, primes[3] = 7, … */
    uint64_t p   = prime_list[pair_idx + 2];
    seq_t    exp = (offset == 0) ? (seq_t)(p - 1) : (seq_t)(p + 1);

    return big_pow((seq_t)p, exp);
}
