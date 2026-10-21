/*
 * A283486  Number of k such that sigma(k) = 2n where sigma(m) = A000203(m) is
 *          the sum of the divisors of m.
 * https://oeis.org/A283486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of k such that σ(k) = 2*n (σ is the
 *            sum‑of‑divisors function)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/*  Private state – lazily built on first use  */
static uint64_t *sigma_arr = NULL;   /* σ(k) for 0 ≤ k ≤ cur_limit   */
static uint64_t *freq_arr  = NULL;   /* count of k with σ(k)=s for 0 ≤ s ≤ cur_limit */
static idx_t    cur_limit = 0;       /* largest index of σ protected */

/*  Rebuild the tables up to at least 'limit'  */
static void recompute(idx_t limit)
{
    /* Allocate new arrays (uint64_t guarantees no overflow for σ(k)  */
    uint64_t *new_sigma = (uint64_t *)malloc((limit + 1) * sizeof(uint64_t));
    uint64_t *new_freq  = (uint64_t *)malloc((limit + 1) * sizeof(uint64_t));
    if (!new_sigma || !new_freq) abort();            /* unrecoverable */

    /* 1. Initialise σ array to 0  */
    for (idx_t i = 0; i <= limit; ++i) new_sigma[i] = 0;

    /* 2. Compute σ(k) by the divisor‑sum sieve  */
    for (idx_t i = 1; i <= limit; ++i) {
        uint64_t inc = (uint64_t)i;
        for (idx_t j = i; j <= limit; j += i)
            new_sigma[j] += inc;
    }

    /* 3. Initialise frequency array to 0  */
    for (idx_t i = 0; i <= limit; ++i) new_freq[i] = 0;

    /* 4. Count how many k give each σ value (only those ≤ limit matter)  */
    for (idx_t k = 1; k <= limit; ++k) {
        uint64_t s = new_sigma[k];
        if (s <= (uint64_t)limit) new_freq[s] += 1;
    }

    /* 5. Replace old tables (free memory if present) */
    free(sigma_arr);
    free(freq_arr);
    sigma_arr = new_sigma;
    freq_arr  = new_freq;
    cur_limit = limit;
}

/*  Public sequence function  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* not defined for n=0 – safety */

    /* Target σ‑value is 2*n  */
    idx_t target = n * 2ULL;
    if (target > cur_limit) recompute(target);

    /* Return how many k have σ(k) = 2*n  */
    return (seq_t)freq_arr[target];
}
