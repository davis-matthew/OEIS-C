/*
 * A324760  Heinz numbers of integer partitions not containing 1 or any part
 *          whose prime indices all belong to the partition.
 * https://oeis.org/A324760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th odd number whose prime factorization
 *            corresponds to a valid partition of its prime indices. A
 *            partition is valid if it does not contain 1 and for each part y,
 *            not all of its prime indices belong to the partition
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define LIMIT 500000        /* upper bound for generating primes */
#define MAX_PRIMES 60000    /* safe upper bound for number of primes */

/* Global prime data */
static int primes[MAX_PRIMES];
static int prime_index[LIMIT + 1];
static int num_primes = 0;

/* Frequency table for part indices, size enough for all indices */
static int freq[MAX_PRIMES];

/* Sieve of Eratosthenes to fill primes[] and prime_index[] */
static void init_primes(void)
{
    static bool is_composite[LIMIT + 1] = { false };
    int i, j, p;

    for (i = 2; i <= LIMIT; ++i)
    {
        if (!is_composite[i])
        {
            if (num_primes >= MAX_PRIMES) break;
            primes[num_primes++] = i;
            prime_index[i] = num_primes; /* 1‑based index */
            if ((uint64_t)i * i <= LIMIT)
                for (j = i * i, p = i; j <= LIMIT; j += p)
                    is_composite[j] = true;
        }
    }
}

/* Helper: factor n and add corresponding part indices to freq[].
   The factorization is for an odd n, so 2 will never appear. */
static void add_part_indices(int n)
{
    int temp = n;
    for (int i = 0; i < num_primes && primes[i] * primes[i] <= temp; ++i)
    {
        int p = primes[i];
        if (temp % p == 0)
        {
            int e = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++e;
            }
            int idx = prime_index[p];
            while (e--)
            {
                ++freq[idx];
            }
        }
    }
    if (temp > 1) /* remaining prime factor */
    {
        int idx = prime_index[temp];
        ++freq[idx];
    }
}

/* Test whether a given odd number n has a valid partition. */
static bool is_valid(int n)
{
    /* reset frequency table */
    memset(freq, 0, sizeof(freq));

    add_part_indices(n);

    /* Find the maximum part index that appears */
    int max_idx = 0;
    for (int i = 2; i < MAX_PRIMES; ++i)
    {
        if (freq[i] > 0 && i > max_idx)
            max_idx = i;
    }

    /* Check each part that appears */
    for (int k = 2; k <= max_idx; ++k)
    {
        if (freq[k] == 0) continue;

        /* Factor k to get its prime indices */
        int temp_k = k;
        bool all_present = true;
        for (int i = 0; i < num_primes && primes[i] * primes[i] <= temp_k; ++i)
        {
            int p = primes[i];
            if (temp_k % p == 0)
            {
                int idx_p = prime_index[p];
                if (freq[idx_p] == 0)
                {
                    all_present = false;
                    break;
                }
                while (temp_k % p == 0) temp_k /= p;
            }
        }
        if (all_present && temp_k > 1)
        {
            int idx_p = prime_index[temp_k];
            if (freq[idx_p] == 0) all_present = false;
        }
        if (all_present) return false; /* invalid partition */
    }
    return true; /* all parts valid */
}

/* Public function required by the OEIS framework */
seq_t a(idx_t n)
{
    /* Ensure primes are initialized once */
    static bool initialized = false;
    if (!initialized)
    {
        init_primes();
        initialized = true;
    }

    if (n == 1) return (seq_t)1; /* empty partition */

    idx_t count = 0;
    int cand = 3; /* start checking from the smallest odd >1 */

    while (true)
    {
        if (is_valid(cand))
        {
            ++count;
            if (count == n - 1)
                return (seq_t)cand;
        }
        cand += 2; /* only odd candidates */
    }
}
