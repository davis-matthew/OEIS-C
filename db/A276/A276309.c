/*
 * A276309  Integer part of the ratio of alternate consecutive prime gaps.
 * https://oeis.org/A276309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((prime(n+3)-prime(n+2))/(prime(n+1)-prime(n)))
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <stdint.h>

/* static storage for primes */
static uint64_t *prime_list = NULL;
static size_t prime_count = 0;
static size_t prime_cap = 0;
static uint64_t next_candidate = 2;

/* generate primes until we have at least `need` primes (1‑based count) */
static void ensure_primes(idx_t need)
{
    if ((size_t)need <= prime_count)
        return;
    if (prime_list == NULL) {
        prime_cap = 16;
        prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        if (!prime_list) abort();
        prime_count = 0;
        next_candidate = 2;
    }
    while (prime_count < (size_t)need) {
        bool prime = true;
        uint64_t limit = (uint64_t) sqrt((double) next_candidate);
        for (size_t i = 0; i < prime_count && prime_list[i] <= limit; ++i) {
            if (next_candidate % prime_list[i] == 0) {
                prime = false;
                break;
            }
        }
        if (prime) {
            if (prime_count == prime_cap) {
                prime_cap <<= 1;
                prime_list = (uint64_t *)realloc(prime_list,
                                                prime_cap * sizeof(uint64_t));
                if (!prime_list) abort();
            }
            prime_list[prime_count++] = next_candidate;
        }
        ++next_candidate;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for n<1, return 0 as a safe value */
    /* we need primes up to index n+3 */
    ensure_primes(n + 3);

    /* indices are 1-based in OEIS */
    uint64_t p1 = prime_list[n - 1];
    uint64_t p2 = prime_list[n];
    uint64_t p3 = prime_list[n + 1];
    uint64_t p4 = prime_list[n + 2];

    uint64_t gap1 = p2 - p1;
    uint64_t gap2 = p4 - p3;
    /* floor division of unsigned integers */
    uint64_t res = gap2 / gap1;
    return (seq_t)res;
}
