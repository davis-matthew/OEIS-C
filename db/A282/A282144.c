/*
 * A282144  Numbers n with k digits in base x (MSD(n)=d_k, LSD(n)=d_1) such
 *          that, chosen one of their digits in position d_k < j < d_1, is
 *          Sum_{i=j..k}{(i-j+1)*d_i} = Sum_{i=1..j-1}{(j-i)*d_i}. Case x = 3.
 * https://oeis.org/A282144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 6487, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes a(n) = the n‑th integer whose base‑3 representation admits a
    fulcrum position j (1 < j ≤ k, k = number of digits, MSD = d_k, LSD = d_1)
    satisfying            Σ_{i=j..k} (i-j+1)·d_i  =  Σ_{i=1..j-1} (j-i)·d_i
    (fulcrum is between two digits).  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* cache for already found terms */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap = 0;
static idx_t last_examined = 0;   /* last natural number that was checked */

/* Append a new term to the cache, enlarging it if necessary. */
static void cache_append(seq_t v)
{
    if (cache_cap == 0) {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else if (cache_size == cache_cap) {
        cache_cap <<= 1;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
    cache[cache_size++] = v;
}

/* Test whether an integer has a fulcrum in its base‑3 digits. */
static bool has_fulcrum(idx_t x)
{
    /* collect digits in little‑endian order (d_1 first) */
    uint8_t dig[128];
    size_t k = 0;
    idx_t y = x;
    do {
        dig[k++] = (uint8_t)(y % 3);
        y /= 3;
    } while (y > 0);

    if (k < 2)               /* need at least one digit on each side of the fulcrum */
        return false;

    seq_t preSum[128] = {0};
    seq_t preT[128]   = {0};
    seq_t preW1[128]  = {0};

    preSum[0] = (seq_t)dig[0];
    preT[0]   = (seq_t)0 * dig[0];
    preW1[0]  = (seq_t)1 * dig[0];

    for (size_t i = 1; i < k; ++i) {
        preSum[i] = preSum[i-1] + (seq_t)dig[i];
        preT[i]   = preT[i-1]   + (seq_t)i * dig[i];
        preW1[i]  = preW1[i-1]  + (seq_t)(i + 1) * dig[i];
    }

    seq_t totalSum = preSum[k-1];
    seq_t totalW1  = preW1[k-1];

    /* try every admissible fulcrum position j (1 < j ≤ k)      */
    /* jp = j-1 runs from 1 to k-1 (0‑based indexing)          */
    for (size_t jp = 1; jp < k; ++jp) {
        seq_t sumRight = totalSum - preSum[jp-1];
        seq_t w1Right  = totalW1  - preW1[jp-1];

        /* left side: (i-j+1)*d_i for i = j..k  */
        seq_t left = w1Right - (seq_t)jp * sumRight;

        /* right side: (j-i)*d_i for i = 1..j-1  */
        seq_t sumLeft = preSum[jp-1];
        seq_t TLeft   = preT[jp-1];
        seq_t right = (seq_t)jp * sumLeft - TLeft;

        if (left == right)
            return true;
    }
    return false;
}

/* Sequence entry function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, placeholder */

    while (cache_size < (size_t)n) {
        ++last_examined;              /* examine next natural number */
        if (has_fulcrum(last_examined))
            cache_append((seq_t)last_examined);
    }
    return cache[n-1];                 /* 0‑based index in the cache */
}
