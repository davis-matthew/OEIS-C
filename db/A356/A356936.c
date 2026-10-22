/*
 * A356936  Number of multiset partitions of the multiset of prime indices of
 *          n into intervals. Number of factorizations of n into members of
 *          A073485.
 * https://oeis.org/A356936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   build_primes_upto, factor_u64, partitions_of_block
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A356936: Number of multiset partitions of the multiset of
   prime indices of n into intervals.
   For each distinct prime factor p^e of n we take the index of p
   (the position of p in the list of primes).  The multiset consists
   of these indices repeated e times.  A partition is a multiset of
   intervals of consecutive integers; each interval must contain
   exactly one copy of every index it covers.  The number of such
   partitions is returned. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/factor_u64.h"
#include "../utils/build_primes_upto.h"
#include "../utils/partitions_of_block.h"

seq_t a(idx_t n)
{
    /* Prime list and index mapping are built only once */
    static bool init_done = false;
    static uint64_t *primes = NULL;
    static size_t prime_cnt = 0;
    static uint16_t prime_idx[10001] = {0};

    if (!init_done) {
        size_t cnt;
        build_primes_upto(10000, &primes, &cnt);   /* primes up to 10000 */
        prime_cnt = cnt;
        for (size_t i = 0; i < prime_cnt; i++) {
            uint64_t p = primes[i];
            if (p <= 10000)
                prime_idx[p] = (uint16_t)(i + 1);  /* 1‑based index */
        }
        init_done = true;
    }

    if (n == 1)
        return (seq_t)1;                 /* empty multiset has one partition */

    /* Factorise n */
    uint64_t pf[10];
    uint64_t ex[10];
    size_t fcnt = 0;
    factor_u64(n, pf, ex, &fcnt);

    /* Count copies of each prime index */
    unsigned int count_by_index[prime_cnt + 1];   /* indices are 1‑based */
    memset(count_by_index, 0, sizeof(count_by_index));
    for (size_t i = 0; i < fcnt; i++) {
        uint64_t p = pf[i];
        unsigned int idx = prime_idx[p];          /* 1‑based index of p */
        count_by_index[idx] += (unsigned int)ex[i];
    }

    /* Find consecutive blocks of non‑zero indices */
    seq_t answer = (seq_t)1;
    unsigned int block_cnt[prime_cnt + 1];   /* temporary storage for block counts */
    int block_len = 0;

    for (size_t i = 1; i <= prime_cnt; i++) {
        unsigned int c = count_by_index[i];
        if (c == 0) {
            if (block_len > 0) {
                seq_t block_res = partitions_of_block(block_cnt, block_len);
                answer *= block_res;
                block_len = 0;
            }
        } else {
            block_cnt[block_len++] = c;
        }
    }
    if (block_len > 0) {
        seq_t block_res = partitions_of_block(block_cnt, block_len);
        answer *= block_res;
    }
    return answer;
}
