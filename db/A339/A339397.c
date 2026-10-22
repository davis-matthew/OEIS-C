/*
 * A339397  Number of partitions of n into an even number of relatively prime
 *          parts.
 * https://oeis.org/A339397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} mu(n/d) * A027187(d) (A027187 = partitions into
 *            an even number of parts)
 * Range:     u8 n <= 19, u16 n <= 47, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static seq_t *part_even = NULL;    /* A027187(n) for 0 <= n <= cap-1 */
static int    *mu_arr    = NULL;    /* Möbius values for the same range */
static idx_t   cap       = 0;        /* current maximum valid index + 1 */

static void ensure_capacity(idx_t n)
{
    if (cap > n) return; /* already sufficient */

    idx_t old_cap = cap;
    idx_t new_cap = n + 1;

    /* Resize Möbius array */
    int *new_mu = (int *)realloc(mu_arr, new_cap * sizeof(int));
    if (!new_mu) { abort(); }
    mu_arr = new_mu;

    /* Resize partition-even array */
    seq_t *new_even = (seq_t *)realloc(part_even, new_cap * sizeof(seq_t));
    if (!new_even) { abort(); }
    part_even = new_even;

    /* Zero-initialize the new cells (POD types only, but zero is a valid value for our purposes) */
    if (old_cap < new_cap) {
        memset((void *)(mu_arr + old_cap), 0, (new_cap - old_cap) * sizeof(int));
        memset((void *)(part_even + old_cap), 0, (new_cap - old_cap) * sizeof(seq_t));
    }

    /* ----------------------- Möbius function ----------------------- */
    /* Linear sieve for μ up to new_cap-1 */
    if (new_cap > 0) {
        mu_arr[0] = 0;     /* unused */
        mu_arr[1] = 1;
    }
    /* We recompute the sieve from scratch for simplicity */
    int *is_comp  = (int *)calloc(new_cap, sizeof(int));
    int *primes   = (int *)calloc(new_cap, sizeof(int));
    int   primes_cnt = 0;
    if (new_cap > 1) {
        for (idx_t i = 2; i < new_cap; ++i) {
            if (!is_comp[i]) {
                primes[primes_cnt++] = (int)i;
                mu_arr[i] = -1;
            }
            for (int j = 0; j < primes_cnt; ++j) {
                idx_t v = i * (idx_t)primes[j];
                if (v >= new_cap) break;
                is_comp[v] = 1;
                if (i % primes[j] == 0) {
                    mu_arr[v] = 0;
                    break;
                } else {
                    mu_arr[v] = -mu_arr[i];
                }
            }
        }
    }
    free(is_comp);
    free(primes);

    /* -------------------- Partitions into an even number of parts -------------------- */
    /* Dynamically compute A027187 for all n < new_cap using a two‑array DP. */
    seq_t *even = (seq_t *)calloc(new_cap, sizeof(seq_t));
    seq_t *odd  = (seq_t *)calloc(new_cap, sizeof(seq_t));
    if (!even || !odd) { abort(); }

    even[0] = (seq_t)1; /* empty partition has even number (0) of parts */
    odd[0]  = (seq_t)0;

    for (idx_t k = 1; k < new_cap; ++k) {
        for (idx_t j = k; j < new_cap; ++j) {
            odd[j]  += even[j - k];
            even[j] += odd[j - k];
        }
    }

    /* Store the result */
    for (idx_t i = 0; i < new_cap; ++i) {
        part_even[i] = even[i];
    }

    free(even);
    free(odd);

    /* Update capacity */
    cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset starts at 1 */

    ensure_capacity(n);

    seq_t result = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t d1 = d;
            idx_t d2 = n / d;

            int mu1 = mu_arr[n / d1];
            if (mu1 == 1) {
                result += part_even[d1];
            } else if (mu1 == -1) {
                result -= part_even[d1];
            }
            if (d1 != d2) {
                int mu2 = mu_arr[n / d2];
                if (mu2 == 1) {
                    result += part_even[d2];
                } else if (mu2 == -1) {
                    result -= part_even[d2];
                }
            }
        }
    }
    return result;
}
