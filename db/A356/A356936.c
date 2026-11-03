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
 * Validated: partial, pass, 87 terms through n = 87 (2026-11-03)
 * Helpers:   build_primes_upto, factor_u64, mem
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
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/factor_u64.h"
#include "../utils/mem.h"

/* Block DP: compute number of partitions of a block of consecutive
   indices with counts 'cnt[0..len-1]'.  The result is the number of
   ways to use all copies, expressed as a seq_t. */
static seq_t partitions_of_block(const unsigned int *cnt, int len)
{
    /* maximum total number of copies in this block */
    int max_copies = 0;
    for (int i = 0; i < len; i++)
        max_copies += cnt[i];

    /* dp_prev and dp_next hold counts for the number of open
       intervals after processing each index.  The number of open
       intervals never exceeds max_copies. */
    seq_t dp_prev[max_copies + 1];
    seq_t dp_next[max_copies + 1];
    mem_set(dp_prev, 0, sizeof(dp_prev));
    dp_prev[0] = (seq_t)1;
    int current_max = 0;              /* maximum open intervals seen so far */

    for (int idx = 0; idx < len; idx++) {
        int ci = cnt[idx];
        int new_max = current_max + ci;   /* max possible after this index */
        mem_set(dp_next, 0, sizeof(seq_t) * (new_max + 1));

        for (int kPrev = 0; kPrev <= current_max; kPrev++) {
            seq_t ways = dp_prev[kPrev];
            if (ways == 0) continue;
            int new_j = ci - kPrev;          /* new intervals started at this index */
            if (new_j < 0) continue;         /* cannot use all copies with current open intervals */

            /* After this index we may have from 0 up to kPrev + new_j
               open intervals, depending on how many of the old and
               new intervals continue.  Since intervals are
               indistinguishable, every choice of the number of
               continuing intervals is a single possibility. */
            int max_t_next = kPrev + new_j;
            for (int tNext = 0; tNext <= max_t_next; tNext++) {
                dp_next[tNext] += ways;
            }
        }
        current_max = new_max;
        /* Copy dp_next to dp_prev for next iteration */
        for (int i = 0; i <= current_max; i++)
            dp_prev[i] = dp_next[i];
    }
    /* All intervals must be closed after the last index */
    return dp_prev[0];
}

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
