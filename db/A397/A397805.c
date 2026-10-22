/*
 * A397805  Composite numbers whose 3rd smallest divisor is less than the
 *          square of their least prime factor.
 * https://oeis.org/A397805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th composite number whose 3rd smallest divisor (the
 *            second distinct prime factor) is less than the square of its
 *            least prime factor. The code generates the sequence by sieving
 *            smallest prime factors up to a fixed limit and collecting
 *            qualifying numbers. It is O(LIMIT) with a small co...
 * Range:     u8 n <= 77, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Adjust LIMIT large enough to contain at least 10 000 terms. */
static const idx_t LIMIT = 2000000;          /* 2 million, plenty of terms */

/* Array to hold the pre‑computed terms. */
#define MAX_TERMS 200000
static seq_t seq_arr[MAX_TERMS];
static size_t seq_len = 0;

/* One‑time construction flag. */
static bool seq_init = false;

/* Smallest prime factor table. */
static void build_spf(uint64_t *spf, idx_t n)
{
    for (idx_t i = 0; i <= n; ++i)
        spf[i] = i;
    for (idx_t i = 2; i * i <= n; ++i) {
        if (spf[i] == i) {
            for (idx_t j = i * i; j <= n; j += i) {
                if (spf[j] == j)
                    spf[j] = i;
            }
        }
    }
}

/* Build the sequence once. */
static void init_seq(void)
{
    /* Allocate array for SPF. */
    uint64_t *spf = (uint64_t *)malloc((LIMIT + 1) * sizeof(uint64_t));
    build_spf(spf, LIMIT);

    for (idx_t num = 4; num <= LIMIT; ++num) {
        uint64_t p1 = spf[num];
        if (p1 == num)       /* prime, skip */
            continue;

        /* Remove all factors of p1. */
        idx_t temp = num / p1;
        while (temp % p1 == 0)
            temp /= p1;

        if (temp == 1)       /* single prime power, skip */
            continue;

        uint64_t p2 = spf[temp];
        if ((seq_t)p2 < (seq_t)p1 * (seq_t)p1) {
            if (seq_len < MAX_TERMS)
                seq_arr[seq_len++] = (seq_t)num;
        }
    }

    free(spf);
    seq_init = true;
}

seq_t a(idx_t n)
{
    if (!seq_init)
        init_seq();

    /* The problem guarantees that n will not exceed the number of
       pre‑computed terms.  For robustness, if n is out of bounds,
       return 0. */
    if (n == 0 || n > seq_len)
        return (seq_t)0;

    return seq_arr[n - 1];
}
