/*
 * A361708  Inverse Moebius transform of nonprimes.
 * https://oeis.org/A361708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Sum_{d|n} A018252(d)
 * Range:     u8 n <= 71, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Static tables for non‑prime values and primes discovered so far */
static seq_t *nonprime_table = NULL;
static size_t nonprime_count = 0;
static size_t nonprime_cap = 0;

static uint64_t *prime_list = NULL;
static size_t prime_count = 0;
static size_t prime_cap = 0;

static uint64_t next_candidate = 1;   /* next integer to test for primality */

/* Ensure that the non‑prime table has at least `need` entries */
static void ensure_nonprime_count(idx_t need)
{
    if (nonprime_count >= need)
        return;

    /* Grow the nonprime table as needed */
    if (nonprime_cap == 0) {
        nonprime_cap = 16;
        nonprime_table = (seq_t *)malloc(nonprime_cap * sizeof(seq_t));
    }

    /* Grow the prime list as needed */
    if (prime_cap == 0) {
        prime_cap = 16;
        prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    }

    while (nonprime_count < need) {
        uint64_t cand = next_candidate++;
        bool is_prime = false;

        if (cand != 1) {
            /* trial division using primes discovered so far */
            bool prime = true;
            for (size_t i = 0; i < prime_count; ++i) {
                uint64_t p = prime_list[i];
                if (p * p > cand)
                    break;
                if (cand % p == 0) {
                    prime = false;
                    break;
                }
            }
            if (prime) {
                /* add to prime list */
                if (prime_count == prime_cap) {
                    prime_cap *= 2;
                    prime_list = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
                }
                prime_list[prime_count++] = cand;
                is_prime = true;
            }
        }

        if (!is_prime) {
            /* store into nonprime table */
            if (nonprime_count == nonprime_cap) {
                nonprime_cap *= 2;
                nonprime_table = (seq_t *)realloc(nonprime_table, nonprime_cap * sizeof(seq_t));
            }
            nonprime_table[nonprime_count++] = (seq_t)cand;
        }
    }
}

seq_t a(idx_t n)
{
    /* Ensure we have the nonprime values up to index n */
    ensure_nonprime_count(n);

    seq_t sum = 0;

    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;

            /* A018252(d) is stored at nonprime_table[d-1] */
            sum += nonprime_table[d1 - 1];

            if (d1 != d2) {
                sum += nonprime_table[d2 - 1];
            }
        }
    }
    return sum;
}
