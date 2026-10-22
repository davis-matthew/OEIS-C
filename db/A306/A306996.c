/*
 * A306996  Number of primes q < prime(n), such that 4*prime(n) + q is prime.
 * https://oeis.org/A306996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes q < prime(n) such that 4*prime(n)+q is
 *            prime
 * Range:     u8 n <= 15, u16 n <= 2741, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static storage for prime table and sieve */
static uint32_t *prime_arr = NULL;  /* list of primes in ascending order */
static size_t prime_cnt = 0;        /* number of primes stored */
static unsigned char *is_comp = NULL; /* composite marker array */
static idx_t sieve_limit = 0;       /* upper bound of sieve */

/* build prime list and sieve up to a fixed limit */
static void init_primes(void)
{
    if (prime_arr != NULL)            /* already done */
        return;

    /*  prime(10000) = 104729; 5×prime(10000) = 523645
       So 600 000 comfortably covers all needed numbers. */
    const idx_t LIM = 600000;
    sieve_limit = LIM;
    is_comp = (unsigned char *)calloc((size_t)LIM + 1, 1);
    if (!is_comp)
        exit(1);                     /* out of memory */

    /* classic sieve of Eratosthenes */
    for (idx_t i = 2; i * i <= LIM; ++i) {
        if (!is_comp[i]) {
            for (idx_t j = i * i; j <= LIM; j += i)
                is_comp[j] = 1;
        }
    }

    /* collect primes into an array */
    size_t cap = 200000;             /* sufficient for 600 000 */
    prime_arr = (uint32_t *)malloc(cap * sizeof(uint32_t));
    if (!prime_arr)
        exit(1);                     /* out of memory */

    for (idx_t i = 2; i <= LIM; ++i) {
        if (!is_comp[i]) {
            if (prime_cnt == cap) {  /* enlarge if necessary */
                cap <<= 1;
                prime_arr = (uint32_t *)realloc(prime_arr, cap * sizeof(uint32_t));
                if (!prime_arr)
                    exit(1);
            }
            prime_arr[prime_cnt++] = (uint32_t)i;
        }
    }
}

/* OEIS sequence A306996 */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                    /* undefined for 0, safety */

    init_primes();                   /* ensure primes are ready */

    /* prime(n) is the (n‑1)-th element of prime_arr (0‑based) */
    seq_t p = prime_arr[n - 1];
    seq_t count = 0;

    /* iterate over primes q < prime(n) */
    for (idx_t i = 0; i + 1 < n; ++i) {
        seq_t q = prime_arr[i];
        seq_t candidate = 4 * p + q;
        if (candidate <= sieve_limit && !is_comp[(size_t)candidate])
            ++count;
    }

    return count;
}
