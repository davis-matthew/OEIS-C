/*
 * A334171  Maximum number of rectangles with prime side lengths that can be
 *          made with up to 2 perpendicular cuts from all p X q rectangles (p,
 *          q prime) such that p + q = 2n.
 * https://oeis.org/A334171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{i=1..n} c(i)·c(2n−i)·[(1−c(i−2))(1−c(2n−i−2)) * +
 *            2·(1−[c(i−2)=c(2n−i−2)]) + 4·c(i−2)·c(2n−i−2)] * where c(k) = 1
 *            iff k is prime, 0 otherwise. * * (The sum runs over all Goldbach
 *            partitions of 2n. The factor inside * the brackets gives the
 *            number of distinct perpendicular cuts that * pr...
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* static sieve data */
static unsigned char *prime_flags = NULL;   /* 0 = composite, 1 = prime  */
static idx_t        prime_limit = 0;         /* highest index sieved  */

/* Rebuild sieve up to >limit if necessary.  Returns true on success. */
static void ensure_prime_sieve(idx_t limit)
{
    if (limit <= prime_limit) return;       /* already fine */

    /* allocate new flag array */
    unsigned char *new_flags = (unsigned char *)calloc((size_t)(limit + 1), sizeof(unsigned char));
    if (!new_flags) exit(EXIT_FAILURE);     /* OOM */

    /* mark all as prime initially */
    for (idx_t i = 2; i <= limit; ++i) new_flags[i] = 1;

    /* sieve */
    for (idx_t p = 2; p * p <= limit; ++p) {
        if (new_flags[p]) {
            for (idx_t m = p * p; m <= limit; m += p)
                new_flags[m] = 0;
        }
    }

    /* replace old array */
    free(prime_flags);
    prime_flags = new_flags;
    prime_limit = limit;
}

/* Check primality with the sieve.  Assumes ensure_prime_sieve was
 * called with a limit ≥ x.  Returns true iff x ≥ 2 and flagged prime. */
static inline bool is_prime(idx_t x)
{
    return x >= 2 && (x <= prime_limit ? prime_flags[x] : false);
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* out of range, return 0 */

    /* prepare sieve for all numbers up to 2n (needed for Goldbach) */
    ensure_prime_sieve(2 * n);

    seq_t total = 0;              /* result accumulator */

    for (idx_t i = 1; i <= n; ++i) {
        idx_t j = 2 * n - i;      /* partner in Goldbach pair */

        if (!is_prime(i) || !is_prime(j)) continue;

        /* c(i-2) and c(j-2) */
        int c1 = (i > 2 && is_prime(i - 2)) ? 1 : 0;
        int c2 = (j > 2 && is_prime(j - 2)) ? 1 : 0;

        /* compute factor according to the formula */
        int factor;
        if (c1 == c2) {
            factor = (c1 == 0) ? 1 : 4;
        } else {
            factor = 2;
        }

        total += (seq_t)factor;
    }

    return total;
}
