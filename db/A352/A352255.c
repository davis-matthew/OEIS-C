/*
 * A352255  Write down the prime numbers 2,3,5,... and write below each one
 *          the preceding nonprimes greater than the previous prime, in
 *          decreasing order. Sums of the antidiagonals give the terms of the
 *          sequence.
 * https://oeis.org/A352255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of numbers on the antidiagonal with i+j = n-1 in the
 *            table constructed by - primes 2,3,5,… in the first row of each
 *            column - below each prime the composite integers between it and
 *            the previous prime, in decreasing order The sequence begins:
 *            2,4,5,11,17,23,38,
 * Range:     u8 n <= 20, u16 n <= 787, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* is_prime(uint64_t) */

static seq_t *S = NULL;          /* sums of antidiagonals, indexed by (i+j) */
static size_t Scar = 0;          /* allocated capacity of S */
static idx_t processed_count = 0;/* number of columns processed */

static idx_t *primes = NULL;     /* array of primes */
static size_t psize = 0;         /* number of primes stored */
static size_t pcap = 0;          /* allocated capacity of primes array */
static idx_t next_candidate = 2; /* next integer to test for primality */

/* Ensure the S array has at least 'need' elements allocated */
static void ensure_S_capacity(size_t need)
{
    if (Scar >= need) return;
    size_t newcap = Scar ? Scar * 2 : 16;
    if (newcap < need) newcap = need;
    seq_t *tmp = (seq_t *)realloc(S, newcap * sizeof(seq_t));
    if (!tmp) abort();
    /* zero the newly added part */
    for (size_t i = Scar; i < newcap; ++i) tmp[i] = 0;
    S = tmp;
    Scar = newcap;
}

/* Ensure the primes array has room for at least 'need' elements */
static void ensure_primes_capacity(size_t need)
{
    if (pcap >= need) return;
    size_t newcap = pcap ? pcap * 2 : 16;
    if (newcap < need) newcap = need;
    idx_t *tmp = (idx_t *)realloc(primes, newcap * sizeof(idx_t));
    if (!tmp) abort();
    primes = tmp;
    pcap = newcap;
}

/* Generate primes until at least 'size' primes are stored */
static void generate_primes_up_to(size_t size)
{
    if (psize >= size) return;
    ensure_primes_capacity(size);
    while (psize < size)
    {
        while (!is_prime(next_candidate)) ++next_candidate;
        primes[psize++] = next_candidate++;
    }
}

/* Process columns from processed_count up to max_col inclusive */
static void process_columns_up_to(idx_t max_col)
{
    while (processed_count <= max_col)
    {
        idx_t j = processed_count;          /* column index = prime index */
        /* Ensure the j-th prime is available */
        generate_primes_up_to((size_t)j + 1);

        idx_t p = primes[j];
        idx_t prev = (j == 0) ? 0 : primes[j - 1];

        /* Add the prime itself (row i = 0) */
        size_t idx0 = (size_t)j;
        ensure_S_capacity(idx0 + 1);
        S[idx0] += (seq_t)p;

        /* Add the composite numbers in decreasing order below the prime */
        size_t i_counter = 1;
        for (idx_t m = p - 1; m > prev; --m)
        {
            if (!is_prime(m))
            {
                size_t idx = (size_t)j + i_counter;
                ensure_S_capacity(idx + 1);
                S[idx] += (seq_t)m;
                ++i_counter;
            }
        }
        ++processed_count;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for offset 1, but guard against underflow */
    idx_t need_col = n - 1;          /* last column needed for a(n) */
    process_columns_up_to(need_col);
    /* S array might be larger due to composite rows; the relevant sum is at index n-1 */
    size_t idx = (size_t)(n - 1);
    return S[idx];
}
