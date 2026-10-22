/*
 * A334434  Heinz number of the n-th integer partition in graded lexicographic
 *          order.
 * https://oeis.org/A334434
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, tabf
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 683, u32 n <= 7860, u64 n <= 7860,
 *            u128 n <= 7860, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A334434: Heinz number of the n‑th integer partition
   in graded lexicographic order.
   The algorithm enumerates integer partitions by their sum (0,1,2,…),
   and within each sum uses graded lexicographic order (parts in non‑decreasing
   order with the smallest part first).  For the n‑th partition it
   multiplies the prime numbers indexed by the parts.  This yields a
   bijection between positive integers and integer partitions. */

/* Required includes for the helper types. */
#include <stdint.h>
#include <stdbool.h>

/* --- Configuration constants -------------------------------------------------- */
#define MAX_SUM        200        /* enough for indices up to about 10⁵ */
#define PRIME_LIMIT    2000       /* enough to generate the first 200 primes */

/* --- Static tables and a flag to run the one‑time initialization -------------- */
static bool   initialized = false;
static uint64_t partitions[MAX_SUM + 1][MAX_SUM + 1];
static uint64_t cum_parts[MAX_SUM + 1];
static unsigned int primes[MAX_SUM + 1] = {0};   /* primes[1] = 2, … */

/* --- One‑time initialization -------------------------------------------------- */
static void init(void)
{
    /* 1. Enumerate the number of partitions of each sum with parts ≤ m */
    for (int m = 0; m <= MAX_SUM; ++m)
        partitions[0][m] = 1;          /* one empty partition */
    for (int s = 1; s <= MAX_SUM; ++s)
        partitions[s][0] = 0;          /* no parts allowed */

    for (int m = 1; m <= MAX_SUM; ++m) {
        for (int s = 1; s <= MAX_SUM; ++s) {
            uint64_t val = partitions[s][m - 1];
            if (s >= m)
                val += partitions[s - m][m];
            partitions[s][m] = val;
        }
    }

    /* 2. Cumulative total of partitions up to each sum */
    cum_parts[0] = partitions[0][0];
    for (int s = 1; s <= MAX_SUM; ++s)
        cum_parts[s] = cum_parts[s - 1] + partitions[s][s];

    /* 3. Generate the first MAX_SUM primes using a simple sieve. */
    bool composite[PRIME_LIMIT + 1] = {false};   /* compile‑time size, no VLA */
    int count = 0;
    for (int i = 2; i <= PRIME_LIMIT && count < MAX_SUM; ++i) {
        if (!composite[i]) {
            ++count;
            primes[count] = (unsigned int)i;
            if (i * i <= PRIME_LIMIT) {
                for (int j = i * i; j <= PRIME_LIMIT; j += i)
                    composite[j] = true;
            }
        }
    }

    initialized = true;
}

/* --- Helper: construct the k‑th partition of sum 'sum' with parts ≤ max_part  */
/* 'parts' will contain the parts in order; *len is updated with the length.      */
static void build_partition(int sum, int max_part,
                            uint64_t k,
                            int *parts, int *len)
{
    if (sum == 0)
        return;
    for (int a = 1; a <= max_part && a <= sum; ++a) {
        uint64_t cnt = partitions[sum - a][a];
        if (k < cnt) {
            parts[(*len)++] = a;
            build_partition(sum - a, a, k, parts, len);
            return;
        }
        k -= cnt;
    }
}

/* --- Main sequence function -------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!initialized)
        init();

    /* Find the sum 'm' that contains the index n.  cum_parts[m-1] < n ≤ cum_parts[m] */
    int m = 0;
    /* n is idx_t (uint64_t), but cum_parts are uint64_t */
    while (m <= MAX_SUM && cum_parts[m] <= (uint64_t)n)
        ++m;

    uint64_t prev_cum = (m > 0) ? cum_parts[m - 1] : 0ULL;
    uint64_t offset  = (uint64_t)n - prev_cum;    /* zero‑based offset inside sum m */

    /* Build the partition at the given offset */
    int parts_arr[MAX_SUM + 1];
    int len = 0;
    build_partition(m, m, offset, parts_arr, &len);

    /* Compute the Heinz number: product of primes indexed by the parts */
    seq_t result = (seq_t)1;
    for (int i = 0; i < len; ++i) {
        result = result * (seq_t)primes[parts_arr[i]];
    }
    return result;
}
