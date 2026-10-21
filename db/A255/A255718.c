/*
 * A255718  Number of length 3 1..(n+2) arrays with no leading partial sum
 *          equal to a prime and no consecutive values equal.
 * https://oeis.org/A255718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   number of length‑3 arrays of 1..(n+2) with no consecutive *
 *            equal entries and no partial sum equal to a prime. * * The
 *            implementation uses a sieve to test primality and a double loop
 *            * over all possible sums s = a + b. For each composite s it
 *            counts * the admissible pairs (a,b) and the number of...
 * Range:     u8 n <= 9, u16 n <= 52, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --- prime sieve and prefix sums ----------------------------------- */

static unsigned char *prime_table  = NULL;   /* prime_table[i] == 1 if i is prime */
static int          *prime_prefix = NULL;   /* prefix sums of prime_table   */
static size_t        prime_size  = 0;       /* allocated size of arrays */

static void make_prime_table(idx_t limit)
{
    if (limit + 1 <= prime_size)            /* already large enough */
        return;

    free(prime_table);
    free(prime_prefix);
    prime_size = (size_t)limit + 1;
    prime_table = (unsigned char *)malloc(prime_size);
    prime_prefix = (int *)malloc(sizeof(int) * prime_size);

    for (size_t i = 0; i < prime_size; ++i)
        prime_table[i] = 1;
    if (prime_size > 0) prime_table[0] = 0;
    if (prime_size > 1) prime_table[1] = 0;

    for (size_t i = 2; i * i < prime_size; ++i) {
        if (prime_table[i]) {
            for (size_t j = i * i; j < prime_size; j += i)
                prime_table[j] = 0;
        }
    }

    prime_prefix[0] = 0;
    for (size_t i = 1; i < prime_size; ++i)
        prime_prefix[i] = prime_prefix[i - 1] + prime_table[i];
}

/* --- sequence function -------------------------------------------- */

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    const idx_t N = n + 2;                     /* maximum entry value */
    const idx_t limit = 3 * N + 5;             /* primes up to 3N */
    make_prime_table(limit);

    uint64_t total = 0;

    for (idx_t s = 2; s <= 2 * N; ++s) {        /* s = a + b */
        if (prime_table[(size_t)s])            /* skip prime sums   */
            continue;

        /* Determine allowed range for b: 1 <= b <= N, 1 <= a = s-b <= N */
        idx_t L = (s > N) ? s - N : 1;
        idx_t R = (s - 1 < N) ? s - 1 : N;
        if (L > R)
            continue;

        int Pnum   = 0;                        /* number of valid pairs (a,b) */
        int Pprime = 0;                        /* pairs with s + b prime */

        for (idx_t b = L; b <= R; ++b) {
            idx_t a = s - b;
            if (a == b) continue;             /* consecutive equal  */
            if (prime_table[(size_t)a]) continue; /* a must not be prime */

            ++Pnum;
            if (prime_table[(size_t)(s + b)])
                ++Pprime;
        }
        if (Pnum == 0)
            continue;

        /* Count primes in the interval (s+1 .. s+N) */
        int primesInterval = prime_prefix[(size_t)(s + N)] -
                             prime_prefix[(size_t)s];
        int base   = (int)N - primesInterval; /* c's with s + c composite */
        int baseM1 = base - 1;                 /* base - 1  (>= 0) */

        uint64_t part = (uint64_t)Pnum * (uint64_t)baseM1;
        total += part + (uint64_t)Pprime;
    }

    return (seq_t)total;
}
